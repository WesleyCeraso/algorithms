#include "test_matrix.h"
#include <src/linearalgebra/matrices/coordinatelistmatrix.h>

typedef ::testing::Types<CoordinateListMatrix<int>,
                         CoordinateListMatrix<double>> CoordinateListMatrixTests;

INSTANTIATE_TYPED_TEST_CASE_P(CoordinateListMatrix, MatrixTest, CoordinateListMatrixTests);

TEST(CoordinateListMatrix, CopyConstructor)
{
    CoordinateListMatrix<int> ci(3, 3);
    ci[0][0] = 1;
    ci[1][1] = 2;
    ci[2][2] = 3;

    CoordinateListMatrix<double> cd(ci);

    EXPECT_EQ(ci, cd);
}
