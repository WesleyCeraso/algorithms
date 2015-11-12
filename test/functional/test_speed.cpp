#include <gtest/gtest.h>
#include <src/linearalgebra/matrices/compressedmatrix.h>
#include <src/linearalgebra/matrices/densematrix.h>
#include <ctime>

TEST(Speed, DenseDense)
{
    size_t tamanho = 200;
    DenseMatrix<double> m1(tamanho, tamanho, 5);
    DenseMatrix<double> m2(tamanho, tamanho, 10);

    DenseMatrix<double> m3 = m1 * m2;
    //std::cout << std::setw(6) << std::setprecision(3) << m3;
}

TEST(Speed, CompressedCompressed)
{
    size_t tamanho = 200;
    CompressedMatrix<double> m1(tamanho, tamanho, 5);
    CompressedMatrix<double> m2(tamanho, tamanho, 10);

    CompressedMatrix<double> m3 = m1 * m2;
    //std::cout << std::setw(6) << std::setprecision(3) << m3;
}
