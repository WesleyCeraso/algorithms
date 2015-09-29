#include <gtest/gtest.h>
#include <src/linearalgebra/solvers/biconjugategradientmethod.h>
#include <src/linearalgebra/matrices/compressedmatrix.h>
#include <src/linearalgebra/matrices/densematrix.h>
#include <src/linearalgebra/matrices/matrixutils.h>

TEST(BiConjugateGradientMethod, solve)
{
    CompressedMatrix<int> coeficients(3, 3);
	coeficients[0][0] = 10;
	coeficients[0][1] = 1;
	coeficients[1][0] = 1;
	coeficients[1][1] = 10;
	coeficients[1][2] = 1;
	coeficients[2][1] = 1;
	coeficients[2][2] = 10;

	DenseMatrix<double> results(3, 1);
	results[0][0] = 11;
	results[1][0] = 11;
	results[2][0] = 1;

    DenseMatrix<double> variables = BiConjugateGradientMethod::solve<DenseMatrix<double>>(coeficients, results, 1e-10, 10);

    EXPECT_NEAR(variables[0][0], 1., 1e-10);
    EXPECT_NEAR(variables[0][1], 1., 1e-10);
    EXPECT_NEAR(variables[0][2], 0., 1e-10);
}
