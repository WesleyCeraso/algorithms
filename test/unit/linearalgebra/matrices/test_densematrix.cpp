#include "test_matrix.h"
#include <src/linearalgebra/matrices/densematrix.h>

typedef ::testing::Types<DenseMatrix<int>,
                         DenseMatrix<double>> DenseMatrixTests;

INSTANTIATE_TYPED_TEST_CASE_P(DenseMatrix, MatrixTest, DenseMatrixTests);

TEST(DenseMatrix, CopyConstructor)
{
    DenseMatrix<int> ci(3, 3);
    ci[0][0] = 1;
    ci[1][1] = 2;
    ci[2][2] = 3;

    DenseMatrix<double> cd(ci);

    EXPECT_EQ(ci, cd);
}
