#include "matrix_io.h"

#include <cstdlib>
#include <iostream>
#include <random>

int main(int argc, char **argv)
{
    if (argc < 7)
    {
        std::cerr << "Usage: " << argv[0]
                  << " rowsA colsA rowsB colsB outA outB [seed]\n";
        return 1;
    }

    int rowsA = std::atoi(argv[1]);
    int colsA = std::atoi(argv[2]);
    int rowsB = std::atoi(argv[3]);
    int colsB = std::atoi(argv[4]);
    std::string outA = argv[5];
    std::string outB = argv[6];

    unsigned int seed = 123;
    if (argc >= 8)
    {
        seed = static_cast<unsigned int>(std::atoi(argv[7]));
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0)
    {
        std::cerr << "Invalid matrix sizes.\n";
        return 1;
    }

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    Matrix A;
    A.rows = rowsA;
    A.cols = colsA;
    A.data.resize(static_cast<size_t>(rowsA) * colsA);
    for (double &v : A.data)
    {
        v = dist(rng);
    }

    Matrix B;
    B.rows = rowsB;
    B.cols = colsB;
    B.data.resize(static_cast<size_t>(rowsB) * colsB);
    for (double &v : B.data)
    {
        v = dist(rng);
    }

    try
    {
        writeMatrix(outA, A);
        writeMatrix(outB, B);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    std::cout << "Wrote " << outA << " and " << outB << "\n";
    return 0;
}
