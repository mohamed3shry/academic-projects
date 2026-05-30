#include "matmul.h"

#include "matrix_io.h"

#include <mpi.h>
#include <stdexcept>
#include <vector>

static void computeCounts(int rows, int size, std::vector<int> &counts, std::vector<int> &displs, int cols)
{
    counts.resize(size);
    displs.resize(size);

    int base = rows / size;
    int rem = rows % size;

    int offset = 0;
    for (int r = 0; r < size; ++r)
    {
        int localRows = base + (r < rem ? 1 : 0);

        counts[r] = localRows * cols;
        displs[r] = offset;
        offset += counts[r];
    }
}

MatmulResult runMatmul(MPI_Comm comm, const std::string &pathA, const std::string &pathB, CommMode mode)
{
    int rank = 0;
    int size = 0;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    Matrix A;
    Matrix B;

    if (rank == 0)
    {
        A = readMatrix(pathA);
        B = readMatrix(pathB);
        if (A.cols != B.rows)
        {
            throw std::runtime_error("A.cols must match B.rows for multiplication");
        }
    }

    int dims[3] = {0, 0, 0};
    if (rank == 0)
    {
        dims[0] = A.rows;
        dims[1] = A.cols;
        dims[2] = B.cols;
    }

    MPI_Bcast(dims, 3, MPI_INT, 0, comm);

    int rowsA = dims[0];
    int colsA = dims[1];
    int colsB = dims[2];

    std::vector<int> sendCounts;
    std::vector<int> displs;
    computeCounts(rowsA, size, sendCounts, displs, colsA);

    int localRows = sendCounts[rank] / colsA;
    std::vector<double> localA(static_cast<size_t>(localRows) * colsA);
    std::vector<double> localC(static_cast<size_t>(localRows) * colsB, 0.0);

    std::vector<double> fullB;
    if (rank == 0)
    {
        fullB = B.data;
    }
    else
    {
        fullB.resize(static_cast<size_t>(colsA) * colsB);
    }

    MPI_Barrier(comm);
    double startTime = MPI_Wtime();

    if (mode == CommMode::Blocking)
    {
        MPI_Scatterv(rank == 0 ? A.data.data() : nullptr, sendCounts.data(), displs.data(), MPI_DOUBLE,
                     localA.data(), sendCounts[rank], MPI_DOUBLE, 0, comm);
        MPI_Bcast(fullB.data(), colsA * colsB, MPI_DOUBLE, 0, comm);
    }
    else if (mode == CommMode::NonBlocking)
    {
        MPI_Request reqs[2];
        MPI_Iscatterv(rank == 0 ? A.data.data() : nullptr, sendCounts.data(), displs.data(), MPI_DOUBLE,
                      localA.data(), sendCounts[rank], MPI_DOUBLE, 0, comm, &reqs[0]);
        MPI_Ibcast(fullB.data(), colsA * colsB, MPI_DOUBLE, 0, comm, &reqs[1]);
        MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
    }
    else // Unsafe - will cause deadlock (ring topology)
    {
        // Unsafe: Ring topology with MPI_Ssend (synchronous send)
        // Each rank sends to next rank, receives from previous
        // All ranks execute Ssend BEFORE Recv - creates circular wait
        int nextRank = (rank + 1) % size;
        int prevRank = (rank - 1 + size) % size;

        // Try to send A data in ring topology using Ssend
        // This blocks all ranks on send waiting for receiver
        // No rank can proceed to receive because all are blocked on send
        MPI_Ssend(localA.data(), sendCounts[rank], MPI_DOUBLE, nextRank, 0, comm);
        // This line never executes due to deadlock above
        MPI_Recv(localA.data(), sendCounts[rank], MPI_DOUBLE, prevRank, 0, comm, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < localRows; ++i)
    {
        for (int k = 0; k < colsA; ++k)
        {
            double a = localA[static_cast<size_t>(i) * colsA + k];
            const double *bRow = &fullB[static_cast<size_t>(k) * colsB];
            double *cRow = &localC[static_cast<size_t>(i) * colsB];
            for (int j = 0; j < colsB; ++j)
            {
                cRow[j] += a * bRow[j];
            }
        }
    }

    std::vector<int> recvCounts;
    std::vector<int> recvDispls;
    computeCounts(rowsA, size, recvCounts, recvDispls, colsB);

    std::vector<double> fullC;
    if (rank == 0)
    {
        fullC.resize(static_cast<size_t>(rowsA) * colsB);
    }

    if (mode == CommMode::Blocking)
    {
        MPI_Gatherv(localC.data(), recvCounts[rank], MPI_DOUBLE,
                    rank == 0 ? fullC.data() : nullptr, recvCounts.data(), recvDispls.data(), MPI_DOUBLE, 0, comm);
    }
    else
    {
        MPI_Request req;
        MPI_Igatherv(localC.data(), recvCounts[rank], MPI_DOUBLE,
                     rank == 0 ? fullC.data() : nullptr, recvCounts.data(), recvDispls.data(), MPI_DOUBLE, 0, comm, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(comm);
    double endTime = MPI_Wtime();

    MatmulResult res;
    res.execTime = endTime - startTime;
    res.rowsA = rowsA;
    res.colsA = colsA;
    res.colsB = colsB;
    res.flopCount = static_cast<long long>(rowsA) * colsA * colsB * 2;

    return res;
}
