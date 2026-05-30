#include "heat_diffusion.h"
#include <algorithm>
#include <cmath>
#include <mpi.h>
#include <vector>
#include <iostream>
#include "matrix_io.h"

#include <cstdlib>
#include <random>

#include <fstream> // Make sure this is at the top of your file

// Helper function to gather the full matrix on Rank 0
static Matrix gatherFullMatrix(MPI_Comm comm, int rank, int size, 
                               const Matrix& A, int localRows, int globalRows, int globalCols)
{
    Matrix fullGrid;
    
    // Only Rank 0 will allocate memory for the full grid
    if (rank == 0)
    {
        fullGrid.rows = globalRows;
        fullGrid.cols = globalCols;
        fullGrid.data.resize(static_cast<size_t>(globalRows) * globalCols);
    }

    // We only want to send the "real" rows, not the ghost cells.
    // So we create a temporary vector containing only the local rows.
    std::vector<double> sendBuf(static_cast<size_t>(localRows) * globalCols);
    for (int i = 0; i < localRows; ++i)
    {
        for (int j = 0; j < globalCols; ++j)
        {
            // Copy from A (which has ghost rows), starting at row 1
            sendBuf[static_cast<size_t>(i) * globalCols + j] = 
                A.data[static_cast<size_t>(i + 1) * globalCols + j];
        }
    }

    // Tell Rank 0 how many rows to expect from each rank
    std::vector<int> recvCounts(size);
    int sendCount = localRows * globalCols;
    MPI_Gather(&sendCount, 1, MPI_INT, recvCounts.data(), 1, MPI_INT, 0, comm);

    // Calculate where each rank' data should be placed in the full grid
    std::vector<int> displs(size, 0);
    if (rank == 0)
    {
        for (int i = 1; i < size; ++i)
        {
            displs[i] = displs[i - 1] + recvCounts[i - 1];
        }
    }

    // Gather all local data to Rank 0
    MPI_Gatherv(sendBuf.data(), sendCount, MPI_DOUBLE,
                fullGrid.data.data(), recvCounts.data(), displs.data(), MPI_DOUBLE,
                0, comm);

    return fullGrid; // Will be empty for ranks other than 0
}





// FIX 1: Pass Matrix& A, not std::vector<double> &A.data
static void exchangeBoundaries(MPI_Comm comm, int rank, int size,
                               Matrix &A, int localRows, int cols,
                               CommMode mode)
{
    int up = (rank == 0) ? MPI_PROC_NULL : rank - 1;
    int down = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

    // FIX 2: Access the raw pointer via A.data.data()
    double *topSend = A.data.data() + cols;
    double *bottomSend = A.data.data() + static_cast<size_t>(localRows) * cols;
    double *topRecv = A.data.data();
    double *bottomRecv = A.data.data() + static_cast<size_t>(localRows + 1) * cols;

    if (mode == CommMode::Blocking)
    {
        MPI_Sendrecv(topSend, cols, MPI_DOUBLE, up, 0,
                     bottomRecv, cols, MPI_DOUBLE, down, 0, comm, MPI_STATUS_IGNORE);
        MPI_Sendrecv(bottomSend, cols, MPI_DOUBLE, down, 1,
                     topRecv, cols, MPI_DOUBLE, up, 1, comm, MPI_STATUS_IGNORE);
    }
    else if (mode == CommMode::NonBlocking)
    {
        MPI_Request reqs[4];
        MPI_Irecv(bottomRecv, cols, MPI_DOUBLE, down, 0, comm, &reqs[0]);
        MPI_Irecv(topRecv, cols, MPI_DOUBLE, up, 1, comm, &reqs[1]);
        MPI_Isend(topSend, cols, MPI_DOUBLE, up, 0, comm, &reqs[2]);
        MPI_Isend(bottomSend, cols, MPI_DOUBLE, down, 1, comm, &reqs[3]);
        MPI_Waitall(4, reqs, MPI_STATUSES_IGNORE);
    }
    else // Unsafe - will cause deadlock (ring topology)
    {
        int nextRank = (rank + 1) % size;
        int prevRank = (rank - 1 + size) % size;

        MPI_Ssend(topSend, cols, MPI_DOUBLE, nextRank, 0, comm);
        // FIX 2: Use A.data.data() here too!
        MPI_Recv(A.data.data(), cols, MPI_DOUBLE, prevRank, 0, comm, MPI_STATUS_IGNORE);
    }
}





HeatResult runHeatDiffusion(MPI_Comm comm, int globalRows, int globalCols, int iters, CommMode mode)
{
    int rank = 0;
    int size = 0;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int base = globalRows / size;
    int rem = globalRows % size;
    int localRows = base + (rank < rem ? 1 : 0);
    int startRow = rank * base + std::min(rank, rem);

    Matrix A;
    A.cols = globalCols;
    A.rows = localRows; 
    A.data.resize(static_cast<size_t>(localRows + 2) * globalCols);
    std::vector<double> next(A.data.size(), 0.0);

    for (int i = 1; i <= localRows; ++i)
    {
        for (int j = 0; j < globalCols; ++j)
        {
            double x = static_cast<double>(startRow + i - 1) / globalRows;
            double y = static_cast<double>(j) / globalCols;
            A.data[static_cast<size_t>(i) * globalCols + j] = std::sin(2.0 * x) * std::cos(2.0 * y);
        }
    }

    MPI_Barrier(comm);
    double startTime = MPI_Wtime();

    for (int t = 0; t < iters; ++t)
    {
        // FIX 1: Pass A, not A.data
        exchangeBoundaries(comm, rank, size, A, localRows, globalCols, mode);

        for (int i = 1; i <= localRows; ++i)
        {
            for (int j = 1; j < globalCols - 1; ++j)
            {
                double up = A.data[static_cast<size_t>(i - 1) * globalCols + j];
                double down = A.data[static_cast<size_t>(i + 1) * globalCols + j];
                double left = A.data[static_cast<size_t>(i) * globalCols + (j - 1)];
                double right = A.data[static_cast<size_t>(i) * globalCols + (j + 1)];
                next[static_cast<size_t>(i) * globalCols + j] = 0.25 * (up + down + left + right);
            }
        }

             std::swap(A.data, next);
        
        if (t == 0 || t == iters - 1)
        {
            // 1. Gather the full grid from all ranks onto Rank 0
            Matrix fullGrid = gatherFullMatrix(comm, rank, size, A, localRows, globalRows, globalCols);

            // 2. Only Rank 0 writes the file
            if (rank == 0)
            {
                try
                {
                    writeMatrix("data/" + std::to_string((int)t) + "_heat.txt", fullGrid);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Rank 0 Error: " << e.what() << "\n";
                }
            }
        }
    }




    MPI_Barrier(comm);
    double endTime = MPI_Wtime();

    HeatResult res;
    res.execTime = endTime - startTime;
    res.totalElements = static_cast<long long>(globalRows) * globalCols;
    res.totalOps = res.totalElements * (iters - 1) * 5;

    return res;
}