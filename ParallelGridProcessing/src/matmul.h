#ifndef MATMUL_H
#define MATMUL_H

#include <mpi.h>
#include <string>

#include "heat_diffusion.h"

struct MatmulResult
{
    double execTime = 0.0;
    int rowsA = 0;
    int colsA = 0;
    int colsB = 0;
    long long flopCount = 0;
};

MatmulResult runMatmul(MPI_Comm comm, const std::string &pathA, const std::string &pathB, CommMode mode);

#endif
