#ifndef MATRIX_IO_H
#define MATRIX_IO_H

#include <string>
#include <vector>

struct Matrix
{
    int rows = 0;
    int cols = 0;
    std::vector<double> data;
};

Matrix readMatrix(const std::string &path);
void writeMatrix(const std::string &path, const Matrix &m);

#endif
