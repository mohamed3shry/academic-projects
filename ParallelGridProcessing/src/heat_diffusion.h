#ifndef HEAT_DIFFUSION_H
#define HEAT_DIFFUSION_H

#include <mpi.h>

enum class CommMode
{
    Blocking,
    NonBlocking,
    Unsafe
};

struct HeatResult
{
    double execTime = 0.0;
    long long totalElements = 0;
    long long totalOps = 0;
};

HeatResult runHeatDiffusion(MPI_Comm comm, int globalRows, int globalCols, int iters, CommMode mode);

#endif
