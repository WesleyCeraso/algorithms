#include <gtest/gtest.h>
#include <src/linearalgebra/matrices/compressedmatrix.h>
#include <src/linearalgebra/matrices/densematrix.h>
#include <src/linearalgebra/matrices/matrixutils.h>
#include <src/linearalgebra/solvers/stabilizedbiconjugategradientmethod.h>
#include <src/linearalgebra/solvers/biconjugategradientmethod.h>
#include <src/linearalgebra/solvers/ludecompositor.h>
#include <src/linearalgebra/solvers/substitutionmethod.h>

#include <ctime>

class CilindricalCapacitor : public testing::Test
{
public:
    CilindricalCapacitor() :
        points(10),
        total_points(points * points - (4 * points - 4) - ((points / 2 - 1) * (points / 2 - 1))),
        coeficients(total_points, total_points),
        constants(total_points, 1)
    {}

    virtual void SetUp()
    {
        unsigned int pos = 0;
        for (unsigned int i = 0; i < points; ++i)
        {
            for (unsigned int j = 0; j < points; ++j)
            {
                if (!i || !j || i == (points - 1) || j == (points - 1) || (i < (points / 2) && j < (points / 2)))
                    continue;

                coeficients[pos][pos] = 4;

                double constant = 0.;

                if (j > 1)
                {
                    if (j != (points / 2) || i >= (points / 2))
                        coeficients[pos][pos - 1] = -(1 - 1 / (2. * j));
                    else
                        constant += (1 - 1 / (2. * j)) * 10.;
                }
                else
                {
                    coeficients[pos][pos] -= (1 - 1 / (2. * j));
                }

                if (j < (points - 2))
                    coeficients[pos][pos + 1] = -(1 + 1 / (2. * j));

                if (i > 1)
                {
                    if (i < (points / 2))
                        coeficients[pos][pos - (points / 2 - 1)] = -1;
                    else if (i != (points / 2) || j >= (points / 2))
                        coeficients[pos][pos - (points - 2)] = -1;
                    else
                        constant += 10.;
                }
                else
                {
                    coeficients[pos][pos] -= 1;
                }

                if (i < (points / 2 - 1))
                    coeficients[pos][pos + (points / 2 - 1)] = -1;
                else if (i < (points - 2))
                    coeficients[pos][pos + (points - 2)] = -1;


                constants[pos][0] = constant;

                ++pos;
            }
        }
    }

protected:
    const unsigned int points;
    const unsigned int total_points;
    DenseMatrix<double> coeficients;
    DenseMatrix<double> constants;
};

TEST_F(CilindricalCapacitor, PotentialEnergy)
{
    DenseMatrix<double> xBiCGSTAB = StabilizedBiConjugateGradientMethod::solve<DenseMatrix<double>>(coeficients, constants, 1e-5, points * points);

    std::pair<DenseMatrix<double>, DenseMatrix<double>> lu = LuDecompositor::decompose<DenseMatrix<double>>(coeficients);
    DenseMatrix<double> auxLu = SubstitutionMethod::forward(lu.first, constants);
    DenseMatrix<double> xLu = SubstitutionMethod::backward(lu.second, auxLu);

    EXPECT_NEAR(MatrixUtils::euclideanNorm(xLu.transpose()), MatrixUtils::euclideanNorm(xBiCGSTAB.transpose()), 1e-4);

    /*DenseMatrix<double> device(points, points);
    unsigned int pos = 0;
    for (unsigned int i = 0; i < points; ++i)
    {
        for (unsigned int j = 0; j < points; ++j)
        {
            if (i == (points - 1) || j == (points - 1))
                continue;
            else if (i < (points / 2) && j < (points / 2))
                device[i][j] = 10;
            else if (!i)
                device[i][j] = variables[j - (points / 2)][0];
            else if (!j)
                device[i][j] = variables[pos][0];
            else
                device[i][j] = variables[pos++][0];
        }
    }
    std::cout << std::setw(6) << std::setprecision(3) << device;*/
}
