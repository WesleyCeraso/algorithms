#include <gtest/gtest.h>
#include <src/linearalgebra/matrices/compressedmatrix.h>
#include <src/linearalgebra/matrices/densematrix.h>
#include <ctime>

TEST(Speed, DenseDense)
{
    size_t tamanho = 1000;
    DenseMatrix<double> m1(tamanho, tamanho, 5);
    DenseMatrix<double> m2(tamanho, tamanho, 10);

    clock_t c1 = clock();

    DenseMatrix<double> m3 = m1 * m2;
    //std::cout << std::setw(6) << std::setprecision(3) << m3;

    clock_t c2 = clock();
    std::cout << "Tempo: " << (c2 - c1) / (double) CLOCKS_PER_SEC << std::endl;
}

TEST(Speed, CompressedCompressed)
{
    size_t tamanho = 500;
    CompressedMatrix<double> m1(tamanho, tamanho, 5);
    CompressedMatrix<double> m2(tamanho, tamanho, 10);

    clock_t c1 = clock();

    CompressedMatrix<double> m3 = m1 * m2;
    //std::cout << std::setw(6) << std::setprecision(3) << m3;

    clock_t c2 = clock();
    std::cout << "Tempo: " << (c2 - c1) / (double) CLOCKS_PER_SEC << std::endl;
}
