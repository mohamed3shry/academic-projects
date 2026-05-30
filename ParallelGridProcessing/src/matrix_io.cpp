#include "matrix_io.h"

#include <fstream>
#include <stdexcept>

Matrix readMatrix(const std::string &path)
{
    std::ifstream in(path);
    if (!in)
    {
        throw std::runtime_error("Failed to open matrix file: " + path);
    }

    Matrix m;
    in >> m.rows >> m.cols;
    if (m.rows <= 0 || m.cols <= 0)
    {
        throw std::runtime_error("Invalid matrix size in: " + path);
    }

    m.data.resize(static_cast<size_t>(m.rows) * m.cols);
    for (double &v : m.data)
    {
        if (!(in >> v))
        {
            throw std::runtime_error("Not enough matrix data in: " + path);
        }
    }
    return m;
}

void writeMatrix(const std::string &path, const Matrix &m)
{
    std::ofstream out(path);
    if (!out)
    {
        throw std::runtime_error("Failed to write matrix file: " + path);
    }

    out << m.rows << " " << m.cols << "\n";
    for (int i = 0; i < m.rows; ++i)
    {
        for (int j = 0; j < m.cols; ++j)
        {
            out << m.data[static_cast<size_t>(i) * m.cols + j];
            out << (j + 1 == m.cols ? '\n' : ' ');
        }
    }
}
