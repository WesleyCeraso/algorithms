#include <gtest/gtest.h>
#include <src/linearalgebra/solvers/substitutionmethod.h>
#include <src/linearalgebra/matrices/compressedmatrix.h>
#include <src/linearalgebra/matrices/densematrix.h>

TEST(SubstitutionMethod, forward)
{
    CompressedMatrix<double> coeficients(3, 3);
    coeficients[0][0] = 1;
    coeficients[1][0] = 1;
    coeficients[1][1] = 1;
    coeficients[2][0] = 1;
    coeficients[2][1] = 1;
    coeficients[2][2] = 1;

    CompressedMatrix<double> constants(3, 1);
    constants[0][0] = 1;
    constants[1][0] = 3;
    constants[2][0] = 6;

    CompressedMatrix<double> variables = SubstitutionMethod::forward(coeficients, constants);

    EXPECT_EQ(variables[0][0], 1.);
    EXPECT_EQ(variables[1][0], 2.);
    EXPECT_EQ(variables[2][0], 3.);
}

TEST(SubstitutionMethod, backward)
{
    CompressedMatrix<double> coeficients(3, 3);
    coeficients[0][0] = 1;
    coeficients[0][1] = 1;
    coeficients[0][2] = 1;
    coeficients[1][1] = 1;
    coeficients[1][2] = 1;
    coeficients[2][2] = 1;

    CompressedMatrix<double> constants(3, 1);
    constants[0][0] = 6;
    constants[1][0] = 3;
    constants[2][0] = 1;

    CompressedMatrix<double> variables = SubstitutionMethod::backward(coeficients, constants);

    EXPECT_EQ(variables[0][0], 3.);
    EXPECT_EQ(variables[1][0], 2.);
    EXPECT_EQ(variables[2][0], 1.);
}
