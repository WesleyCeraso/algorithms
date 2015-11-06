#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H

#include <gtest/gtest.h>
#include <src/linearalgebra/matrices/matrix.h>
#include <src/linearalgebra/matrices/matrixutils.h>

template <typename T>
class MatrixTest : public ::testing::Test {};

TYPED_TEST_CASE_P(MatrixTest);

TYPED_TEST_P(MatrixTest, identity)
{
	typedef typename TypeParam::value_type value_type;

    EXPECT_DEATH(MatrixUtils::identity<TypeParam>(0, 1), ".*");

    TypeParam myIdentity(3, 3);
    myIdentity[0][0] = value_type(1.);
    myIdentity[1][1] = value_type(1.);
    myIdentity[2][2] = value_type(1.);

    EXPECT_EQ(myIdentity, MatrixUtils::identity<TypeParam>(3, value_type(1.)));

    myIdentity = myIdentity * 5;

    EXPECT_EQ(MatrixUtils::identity<TypeParam>(3, value_type(5.)), myIdentity);
}

TYPED_TEST_P(MatrixTest, iterator)
{
	typedef typename TypeParam::value_type value_type;

    TypeParam matrix(3, 3);
    matrix[0][0] = value_type(1.);
    matrix[1][1] = value_type(2.);
    matrix[2][2] = value_type(3.);

    typename TypeParam::const_iterator beg = matrix.begin();
    EXPECT_EQ(beg, matrix.beginNonZero());
    EXPECT_EQ(*beg, value_type(1.));

    EXPECT_EQ(*beg++, matrix[0][0]);
    EXPECT_EQ(*++beg, matrix[0][2]);
    EXPECT_EQ(*beg, 0);

    EXPECT_EQ(*beg.nextNonZero(), value_type(2.));

    EXPECT_EQ(*beg.nextNonZero(), value_type(3.));

    EXPECT_EQ(beg.nextNonZero(), matrix.endNonZero());
    EXPECT_EQ(beg, matrix.end());
}

TYPED_TEST_P(MatrixTest, constructor)
{
    EXPECT_DEATH(TypeParam(0, 0), ".*");
    EXPECT_DEATH(TypeParam(0, 1), ".*");
    EXPECT_DEATH(TypeParam(1, 0), ".*");

    TypeParam sm(1, 1);
}

TYPED_TEST_P(MatrixTest, operator_At)
{
	typedef typename TypeParam::value_type value_type;

    TypeParam sm(1, 3);

    EXPECT_EQ(sm[0][0], value_type(0.));
    EXPECT_EQ(sm[0][1], value_type(0.));
    EXPECT_EQ(sm[0][2], value_type(0.));

	sm[0][0] = value_type(5.);
    sm[0][1] = value_type(6.);
    sm[0][2] = value_type(7.);

    EXPECT_EQ(sm[0][0], value_type(5.));
    EXPECT_EQ(sm[0][1], value_type(6.));
    EXPECT_EQ(sm[0][2], value_type(7.));
}

TYPED_TEST_P(MatrixTest, rows)
{
    TypeParam sm1(1, 1);
    EXPECT_EQ(sm1.rows(), 1);

    TypeParam sm2(2, 1);
    EXPECT_EQ(sm2.rows(), 2);

    TypeParam sm3(10, 1);
    EXPECT_EQ(sm3.rows(), 10);
}

TYPED_TEST_P(MatrixTest, columns)
{
    TypeParam sm1(1, 1);
    EXPECT_EQ(sm1.columns(), 1);

    TypeParam sm2(1, 2);
    EXPECT_EQ(sm2.columns(), 2);

    TypeParam sm3(1, 10);
    EXPECT_EQ(sm3.columns(), 10);
}

TYPED_TEST_P(MatrixTest, transpose)
{
	typedef typename TypeParam::value_type value_type;

    TypeParam matrix1(3, 4);

    matrix1[0][0] = value_type(0.);
    matrix1[0][1] = value_type(1.);
	matrix1[0][2] = value_type(2.);
    matrix1[0][3] = value_type(9.);
    matrix1[1][0] = value_type(3.);
    matrix1[1][1] = value_type(4.);
    matrix1[1][2] = value_type(5.);
    matrix1[1][3] = value_type(10.);
    matrix1[2][0] = value_type(6.);
    matrix1[2][1] = value_type(7.);
    matrix1[2][2] = value_type(8.);
    matrix1[2][3] = value_type(11.);

    TypeParam transpose1 = matrix1.transpose();

    EXPECT_EQ(transpose1[0][0], matrix1[0][0]);
    EXPECT_EQ(transpose1[0][1], matrix1[1][0]);
    EXPECT_EQ(transpose1[0][2], matrix1[2][0]);
    EXPECT_EQ(transpose1[1][0], matrix1[0][1]);
    EXPECT_EQ(transpose1[1][1], matrix1[1][1]);
    EXPECT_EQ(transpose1[1][2], matrix1[2][1]);
    EXPECT_EQ(transpose1[2][0], matrix1[0][2]);
    EXPECT_EQ(transpose1[2][1], matrix1[1][2]);
    EXPECT_EQ(transpose1[2][2], matrix1[2][2]);
    EXPECT_EQ(transpose1[3][0], matrix1[0][3]);
    EXPECT_EQ(transpose1[3][1], matrix1[1][3]);
    EXPECT_EQ(transpose1[3][2], matrix1[2][3]);
}

TYPED_TEST_P(MatrixTest, operatorEquality)
{
	typedef typename TypeParam::value_type value_type;

    TypeParam matrix1(MatrixUtils::identity<TypeParam>(3, value_type(1.)));
    TypeParam matrix2(MatrixUtils::identity<TypeParam>(3, value_type(5.)));
    TypeParam matrix3(MatrixUtils::identity<TypeParam>(3, value_type(5.)));

	EXPECT_NE(matrix1, matrix2);
	matrix1 = matrix1 * 5;
	EXPECT_EQ(matrix1, matrix2);
	EXPECT_EQ(matrix1, matrix3);
}

TYPED_TEST_P(MatrixTest, operatorMultiplication)
{
	typedef typename TypeParam::value_type value_type;

    TypeParam matrix1(3, 3);
    TypeParam matrix2(3, 3);

	matrix1[0][0] = value_type(5.);
	matrix1[1][1] = value_type(5.);
	matrix1[2][2] = value_type(5.);

	matrix2[0][0] = value_type(10.);
	matrix2[1][1] = value_type(100.);
	matrix2[2][2] = value_type(1000.);

    TypeParam  matrix3 = matrix1 * matrix2;

	EXPECT_EQ(matrix1[0][0], value_type(5.));
	EXPECT_EQ(matrix1[1][1], value_type(5.));
	EXPECT_EQ(matrix1[2][2], value_type(5.));

	EXPECT_EQ(matrix2[0][0], value_type(10.));
	EXPECT_EQ(matrix2[1][1], value_type(100.));
	EXPECT_EQ(matrix2[2][2], value_type(1000.));

	EXPECT_EQ(matrix3[0][0], value_type(50.));
	EXPECT_EQ(matrix3[1][1], value_type(500.));
	EXPECT_EQ(matrix3[2][2], value_type(5000.));

    TypeParam matrix4(3, 3);

	matrix4[0][0] = value_type(1.);
	matrix4[1][1] = value_type(2.);
	matrix4[2][2] = value_type(3.);

    TypeParam matrix5 = matrix3 * matrix4;

	EXPECT_EQ(matrix5[0][0], value_type(50.));
	EXPECT_EQ(matrix5[1][1], value_type(1000.));
	EXPECT_EQ(matrix5[2][2], value_type(15000.));

    TypeParam matrix6 = matrix5 * 15;
    TypeParam matrix7 = 15 * matrix5;

	EXPECT_EQ(matrix6[0][0], value_type(750.));
	EXPECT_EQ(matrix6[1][1], value_type(15000.));
	EXPECT_EQ(matrix6[2][2], value_type(225000.));

	EXPECT_EQ(matrix6[0][0], matrix7[0][0]);
	EXPECT_EQ(matrix6[1][1], matrix7[1][1]);
    EXPECT_EQ(matrix6[2][2], matrix7[2][2]);
}

TYPED_TEST_P(MatrixTest, operatorAddition)
{
	typedef typename TypeParam::value_type value_type;

    TypeParam matrix1(3, 3);
    TypeParam matrix2(3, 3);

	matrix1[0][0] = value_type(5.);
	matrix1[1][1] = value_type(5.);
	matrix1[2][2] = value_type(5.);

	matrix2[0][0] = value_type(10.);
	matrix2[1][1] = value_type(100.);
	matrix2[2][2] = value_type(1000.);

    TypeParam  matrix3 = matrix1 + matrix2;

	EXPECT_EQ(matrix1[0][0], value_type(5.));
	EXPECT_EQ(matrix1[1][1], value_type(5.));
	EXPECT_EQ(matrix1[2][2], value_type(5.));

	EXPECT_EQ(matrix2[0][0], value_type(10.));
	EXPECT_EQ(matrix2[1][1], value_type(100.));
	EXPECT_EQ(matrix2[2][2], value_type(1000.));

	EXPECT_EQ(matrix3[0][0], value_type(15.));
	EXPECT_EQ(matrix3[1][1], value_type(105.));
	EXPECT_EQ(matrix3[2][2], value_type(1005.));

    TypeParam matrix4(3, 3);

	matrix4[0][0] = value_type(1.);
	matrix4[1][1] = value_type(2.);
	matrix4[2][2] = value_type(3.);

    TypeParam matrix5 = matrix3 + matrix4;

	EXPECT_EQ(matrix5[0][0], value_type(16.));
	EXPECT_EQ(matrix5[1][1], value_type(107.));
	EXPECT_EQ(matrix5[2][2], value_type(1008.));

    TypeParam matrix6 = matrix5 + 15;
    TypeParam matrix7 = 15 + matrix5;

	EXPECT_EQ(matrix6[0][0], value_type(31.));
	EXPECT_EQ(matrix6[1][1], value_type(122.));
	EXPECT_EQ(matrix6[2][2], value_type(1023.));

	EXPECT_EQ(matrix6[0][0], matrix7[0][0]);
	EXPECT_EQ(matrix6[1][1], matrix7[1][1]);
	EXPECT_EQ(matrix6[2][2], matrix7[2][2]);

    TypeParam matrix8 = -10 + matrix5;
    TypeParam matrix9 = matrix5 + -10;

    EXPECT_EQ(matrix8[0][0], value_type(6.));
    EXPECT_EQ(matrix8[1][1], value_type(97.));
    EXPECT_EQ(matrix8[2][2], value_type(998.));

    EXPECT_EQ(matrix8[0][0], matrix9[0][0]);
    EXPECT_EQ(matrix8[1][1], matrix9[1][1]);
    EXPECT_EQ(matrix8[2][2], matrix9[2][2]);
}

TYPED_TEST_P(MatrixTest, operatorSubtraction)
{
	typedef typename TypeParam::value_type value_type;

    TypeParam matrix1(3, 3);
    TypeParam matrix2(3, 3);

	matrix1[0][0] = value_type(5.);
	matrix1[1][1] = value_type(5.);
	matrix1[2][2] = value_type(5.);

	matrix2[0][0] = value_type(10.);
	matrix2[1][1] = value_type(100.);
	matrix2[2][2] = value_type(1000.);

    TypeParam matrix3 = matrix1 - matrix2;

	EXPECT_EQ(matrix1[0][0], value_type(5.));
	EXPECT_EQ(matrix1[1][1], value_type(5.));
	EXPECT_EQ(matrix1[2][2], value_type(5.));

	EXPECT_EQ(matrix2[0][0], value_type(10.));
	EXPECT_EQ(matrix2[1][1], value_type(100.));
	EXPECT_EQ(matrix2[2][2], value_type(1000.));

	EXPECT_EQ(matrix3[0][0], value_type(-5.));
	EXPECT_EQ(matrix3[1][1], value_type(-95.));
	EXPECT_EQ(matrix3[2][2], value_type(-995.));

    TypeParam matrix4(3, 3);

	matrix4[0][0] = value_type(1.);
	matrix4[1][1] = value_type(2.);
	matrix4[2][2] = value_type(3.);

    TypeParam matrix5 = matrix3 - matrix4;

	EXPECT_EQ(matrix5[0][0], value_type(-6.));
	EXPECT_EQ(matrix5[1][1], value_type(-97.));
	EXPECT_EQ(matrix5[2][2], value_type(-998.));

    TypeParam matrix6 = matrix5 - 15;
    TypeParam matrix7 = 15 - matrix5;

	EXPECT_EQ(matrix6[0][0], value_type(-21.));
	EXPECT_EQ(matrix6[1][1], value_type(-112.));
	EXPECT_EQ(matrix6[2][2], value_type(-1013.));

	EXPECT_EQ(matrix6[0][0], -matrix7[0][0]);
	EXPECT_EQ(matrix6[1][1], -matrix7[1][1]);
	EXPECT_EQ(matrix6[2][2], -matrix7[2][2]);

    TypeParam matrix8 = -10 - matrix5;
    TypeParam matrix9 = matrix5 - -10;

    EXPECT_EQ(matrix8[0][0], value_type(-4.));
    EXPECT_EQ(matrix8[1][1], value_type(87.));
    EXPECT_EQ(matrix8[2][2], value_type(988.));

    EXPECT_EQ(matrix8[0][0], -matrix9[0][0]);
    EXPECT_EQ(matrix8[1][1], -matrix9[1][1]);
    EXPECT_EQ(matrix8[2][2], -matrix9[2][2]);
}

REGISTER_TYPED_TEST_CASE_P(MatrixTest,
                           identity,
                           iterator,
                           constructor,
                           operator_At,
                           rows,
                           columns,
                           transpose,
                           operatorEquality,
                           operatorMultiplication,
                           operatorAddition,
                           operatorSubtraction);

#endif
