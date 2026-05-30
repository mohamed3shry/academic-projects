#include "heat_diffusion.h"
#include "matmul.h"

#include <mpi.h>

#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

struct Options
{
    std::string algo = "heat";
    std::string comm = "blocking";
    int gridRows = 256;
    int gridCols = 256;
    int iters = 100;
    std::string inputA = "data/A.txt";
    std::string inputB = "data/B.txt";
    std::string logPath = "run.log";
};

static void printUsage(const char *prog)
{
    if (prog == nullptr)
    {
        return;
    }
    std::cout << "Usage: " << prog
              << " --algo heat|matmul|both --comm blocking|nonblocking|unsafe"
              << " --grid R C --iters N --inputA path --inputB path --log path\n";
}

static Options parseArgs(int argc, char **argv)
{
    Options opt;
    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--algo") == 0 && i + 1 < argc)
        {
            opt.algo = argv[++i];
        }
        else if (std::strcmp(argv[i], "--comm") == 0 && i + 1 < argc)
        {
            opt.comm = argv[++i];
        }
        else if (std::strcmp(argv[i], "--grid") == 0 && i + 2 < argc)
        {
            opt.gridRows = std::atoi(argv[++i]);
            opt.gridCols = std::atoi(argv[++i]);
        }
        else if (std::strcmp(argv[i], "--iters") == 0 && i + 1 < argc)
        {
            opt.iters = std::atoi(argv[++i]);
        }
        else if (std::strcmp(argv[i], "--inputA") == 0 && i + 1 < argc)
        {
            opt.inputA = argv[++i];
        }
        else if (std::strcmp(argv[i], "--inputB") == 0 && i + 1 < argc)
        {
            opt.inputB = argv[++i];
        }
        else if (std::strcmp(argv[i], "--log") == 0 && i + 1 < argc)
        {
            opt.logPath = argv[++i];
        }
    }
    return opt;
}

static CommMode parseComm(const std::string &mode)
{
    if (mode == "blocking")
    {
        return CommMode::Blocking;
    }
    if (mode == "nonblocking")
    {
        return CommMode::NonBlocking;
    }
    if (mode == "unsafe")
    {
        return CommMode::Unsafe;
    }
    throw std::runtime_error("Unknown comm mode. Use blocking, nonblocking, or unsafe");
}

static std::string nowString()
{
    std::time_t t = std::time(nullptr);
    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buf);
}

static std::string buildLogLine(const Options &opt, int worldSize, const HeatResult &hres)
{
    std::ostringstream out;
    out << nowString()
        << " | algo=heat"
        << " | comm=" << opt.comm
        << " | procs=" << worldSize
        << " | grid=" << opt.gridRows << "x" << opt.gridCols
        << " | iters=" << opt.iters
        << " | time=" << hres.execTime << "s"
        << " | elements=" << hres.totalElements
        << " | ops=" << hres.totalOps
        << " | ops/sec=" << (hres.execTime > 0 ? hres.totalOps / hres.execTime : 0)
        << "\n";
    return out.str();
}

static std::string buildMatmulLogLine(const Options &opt, int worldSize, const MatmulResult &mres)
{
    std::ostringstream out;
    out << nowString()
        << " | algo=matmul"
        << " | comm=" << opt.comm
        << " | procs=" << worldSize
        << " | A_shape=" << mres.rowsA << "x" << mres.colsA
        << " | B_shape=" << mres.colsA << "x" << mres.colsB
        << " | time=" << mres.execTime << "s"
        << " | flops=" << mres.flopCount
        << " | gflops=" << (mres.execTime > 0 ? mres.flopCount / 1e9 / mres.execTime : 0)
        << "\n";
    return out.str();
}

static void appendLog(const std::string &path, const std::string &line)
{
    std::ofstream out(path, std::ios::app);
    if (!out)
    {
        throw std::runtime_error("Failed to open log file: " + path);
    }
    out << line;
}

static std::string buildStartLogLine(const std::string &algoLabel, int worldSize, int numGroups = 1)
{
    std::ostringstream out;
    out << nowString() << " | [START] algo=" << algoLabel << " | total_procs=" << worldSize << " | groups=" << numGroups << "\n";
    return out.str();
}

static std::string buildGroupAssignmentLine(int worldRank, int localRank, int groupSize, const std::string &algoLabel)
{
    std::ostringstream out;
    out << nowString() << " | [GROUP] rank=" << worldRank << " -> local_rank=" << localRank
        << " | algo=" << algoLabel << " | group_size=" << groupSize << "\n";
    return out.str();
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int worldRank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    Options opt = parseArgs(argc, argv);
    if (worldSize < 2)
    {
        if (worldRank == 0)
        {
            std::cerr << "Need at least 2 processes.\n";
            printUsage(argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    CommMode mode;
    try
    {
        mode = parseComm(opt.comm);
    }
    catch (const std::exception &e)
    {
        if (worldRank == 0)
        {
            std::cerr << e.what() << "\n";
        }
        MPI_Finalize();
        return 1;
    }

    MPI_Comm algoComm = MPI_COMM_NULL;

    if (opt.algo == "both")
    {
        int half = worldSize / 2;
        int color = (worldRank < half) ? 0 : 1;
        MPI_Comm_split(MPI_COMM_WORLD, color, worldRank, &algoComm);

        int localRank = 0;
        int localSize = 0;
        MPI_Comm_rank(algoComm, &localRank);
        MPI_Comm_size(algoComm, &localSize);

        std::string algoName = (color == 0) ? "heat" : "matmul";

        if (worldRank == 0)
        {
            appendLog(opt.logPath, buildStartLogLine("both", worldSize, 2));
        }

        if (localRank == 0)
        {
            appendLog(opt.logPath, buildGroupAssignmentLine(worldRank, localRank, localSize, algoName));
        }

        MPI_Barrier(MPI_COMM_WORLD);

        std::string line;
        if (color == 0)
        {
            HeatResult hres = runHeatDiffusion(algoComm, opt.gridRows, opt.gridCols, opt.iters, mode);
            if (localRank == 0)
            {
                line = buildLogLine(opt, worldSize, hres);
            }
        }
        else
        {
            MatmulResult mres = runMatmul(algoComm, opt.inputA, opt.inputB, mode);
            if (localRank == 0)
            {
                line = buildMatmulLogLine(opt, worldSize, mres);
            }
        }

        if (worldRank == 0)
        {
            if (!line.empty())
            {
                appendLog(opt.logPath, line);
            }
            if (half < worldSize)
            {
                int msgLen = 0;
                MPI_Recv(&msgLen, 1, MPI_INT, half, 200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (msgLen > 0)
                {
                    std::string other(static_cast<size_t>(msgLen), '\0');
                    MPI_Recv(other.data(), msgLen, MPI_CHAR, half, 201, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    appendLog(opt.logPath, other);
                }
            }
        }
        else if (worldRank == half && localRank == 0)
        {
            int msgLen = static_cast<int>(line.size());
            MPI_Send(&msgLen, 1, MPI_INT, 0, 200, MPI_COMM_WORLD);
            if (msgLen > 0)
            {
                MPI_Send(line.data(), msgLen, MPI_CHAR, 0, 201, MPI_COMM_WORLD);
            }
        }
    }
    else if (opt.algo == "heat")
    {
        MPI_Comm_split(MPI_COMM_WORLD, 0, worldRank, &algoComm);

        int localRank = 0;
        int localSize = 0;
        MPI_Comm_rank(algoComm, &localRank);
        MPI_Comm_size(algoComm, &localSize);

        if (localRank == 0)
        {
            appendLog(opt.logPath, buildStartLogLine("heat", worldSize, 1));
        }

        HeatResult hres = runHeatDiffusion(algoComm, opt.gridRows, opt.gridCols, opt.iters, mode);

        if (localRank == 0)
        {
            appendLog(opt.logPath, buildLogLine(opt, worldSize, hres));
        }
    }
    else if (opt.algo == "matmul")
    {
        MPI_Comm_split(MPI_COMM_WORLD, 0, worldRank, &algoComm);

        int localRank = 0;
        int localSize = 0;
        MPI_Comm_rank(algoComm, &localRank);
        MPI_Comm_size(algoComm, &localSize);

        if (localRank == 0)
        {
            appendLog(opt.logPath, buildStartLogLine("matmul", worldSize, 1));
        }

        MatmulResult mres = runMatmul(algoComm, opt.inputA, opt.inputB, mode);

        if (localRank == 0)
        {
            appendLog(opt.logPath, buildMatmulLogLine(opt, worldSize, mres));
        }
    }
    else
    {
        if (worldRank == 0)
        {
            std::cerr << "Unknown algorithm. Use: heat, matmul, or both\n";
            printUsage(argv[0]);
        }
    }

    if (algoComm != MPI_COMM_NULL)
    {
        MPI_Comm_free(&algoComm);
    }

    MPI_Finalize();
    return 0;
}
