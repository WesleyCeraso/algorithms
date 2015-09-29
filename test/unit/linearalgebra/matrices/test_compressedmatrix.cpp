#include "test_matrix.h"
#include <src/linearalgebra/matrices/compressedmatrix.h>

typedef ::testing::Types<CompressedMatrix<int>,
                         CompressedMatrix<double>> CompressedMatrixTests;

INSTANTIATE_TYPED_TEST_CASE_P(CompressedMatrix, MatrixTest, CompressedMatrixTests);

TEST(CompressedMatrix, CopyConstructor)
{
    CompressedMatrix<int> ci(3, 3);
    ci[0][0] = 1;
    ci[1][1] = 2;
    ci[2][2] = 3;

    CompressedMatrix<double> cd(ci);

    EXPECT_EQ(ci, cd);
}
