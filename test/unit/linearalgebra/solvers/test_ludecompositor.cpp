#include <gtest/gtest.h>
#include <src/linearalgebra/matrices/compressedmatrix.h>
#include <src/linearalgebra/matrices/densematrix.h>
#include <src/linearalgebra/solvers/ludecompositor.h>

TEST(LuDecompositor, decompose)
{
    DenseMatrix<double> coeficients(3, 3);
	coeficients[0][0] = 10;
	coeficients[0][1] = 1;
	coeficients[1][0] = 1;
	coeficients[1][1] = 10;
	coeficients[1][2] = 1;
	coeficients[2][1] = 1;
    coeficients[2][2] = 10;

    std::pair<DenseMatrix<double>, DenseMatrix<double>> lu = LuDecompositor::decompose<DenseMatrix<double>>(coeficients);

    DenseMatrix<double> result = lu.first * lu.second;

    EXPECT_EQ(coeficients, result);
}
