#include <gtest/gtest.h>
#include <src/linearalgebra/matrices/compressedmatrix.h>
#include <src/linearalgebra/matrices/densematrix.h>
#include <src/linearalgebra/matrices/matrixutils.h>
#include <src/linearalgebra/matrices/pointermatrix.h>
#include <src/linearalgebra/solvers/stabilizedbiconjugategradientmethod.h>
#include <src/linearalgebra/solvers/conjugategradientmethod.h>
#include <src/linearalgebra/solvers/biconjugategradientmethod.h>
#include <src/linearalgebra/solvers/ludecompositor.h>
#include <src/linearalgebra/solvers/substitutionmethod.h>
#include <fstream>
#include <ctime>
#include <iomanip>

unsigned int linhas_colunas = 50;
unsigned int vertices_linha = linhas_colunas + 1;
unsigned int vertices_total = vertices_linha * vertices_linha;

class Point
{
public:
    double x, y;
};

class DegreeOfFreedom
{
public:
    enum BoundaryCondition {
        None = 0,
        Dirichlet = 1,
        Neumann = 2
    };

    BoundaryCondition bc;
    Point point;
    double valor;
};

class FiniteElement
{
public:
    FiniteElement(const std::vector<const DegreeOfFreedom*> & dofs):
        m_degreesOfFreedom(dofs),
        m_stiffness(3, 3),
        m_constants(3, 1)
    {
        DenseMatrix<double> c(2, 3);
        c[0][0] = m_degreesOfFreedom[1]->point.y - m_degreesOfFreedom[2]->point.y;
        c[0][1] = m_degreesOfFreedom[2]->point.y - m_degreesOfFreedom[0]->point.y;
        c[0][2] = m_degreesOfFreedom[0]->point.y - m_degreesOfFreedom[1]->point.y;
        c[1][0] = m_degreesOfFreedom[2]->point.x - m_degreesOfFreedom[1]->point.x;
        c[1][1] = m_degreesOfFreedom[0]->point.x - m_degreesOfFreedom[2]->point.x;
        c[1][2] = m_degreesOfFreedom[1]->point.x - m_degreesOfFreedom[0]->point.x;
        m_stiffness = c.transpose() * (DenseMatrix<double>)(c * 0.5);
    }

    void applyBoundaryConditions()
    {
        for (unsigned int i = 0; i < m_degreesOfFreedom.size(); ++i)
        {
            if (m_degreesOfFreedom[i]->bc == DegreeOfFreedom::Dirichlet)
            {
                m_constants[i][0] = m_degreesOfFreedom[i]->valor * m_stiffness[i][i];
                for (unsigned int j = 0; j < m_degreesOfFreedom.size(); ++j)
                {
                    if (i == j) continue;
                    if (m_degreesOfFreedom[j]->bc != DegreeOfFreedom::Dirichlet)
                    {
                        m_constants[j][0] += -(m_degreesOfFreedom[i]->valor) * m_stiffness[j][i];
                    }
                    m_stiffness[i][j] = m_stiffness[j][i] = 0;
                }
            }
        }
    }

    DenseMatrix<double> constants() const
    {
        return m_constants;
    }

    DenseMatrix<double> stiffness()
    {
        return m_stiffness;
    }

    std::vector<const DegreeOfFreedom*> m_degreesOfFreedom;
    DenseMatrix<double> m_stiffness;
    DenseMatrix<double> m_constants;
};

std::vector<DegreeOfFreedom> getDegreesOfFreedom()
{
    std::vector<DegreeOfFreedom> dof;
    for (unsigned int i = 0; i < vertices_linha; ++i)
    {
        for (unsigned int j = 0; j < vertices_linha; ++j)
        {
            DegreeOfFreedom d;
            d.point.x = i;
            d.point.y = j;

            if (i < (vertices_linha / 2) && j < (vertices_linha / 2))
            {
                d.valor = 10;
                d.bc = DegreeOfFreedom::Dirichlet;
            }
            else if (i == (vertices_linha - 1) || j == (vertices_linha - 1))
            {
                d.valor = 0;
                d.bc = DegreeOfFreedom::Dirichlet;
            }
            else
            {
                d.valor = 0;
                d.bc = DegreeOfFreedom::None;
            }

            dof.push_back(d);
        }
    }

    /*for (unsigned int i = 0; i < vertices_linha; ++i)
    {
        dof[i].bc = DegreeOfFreedom::Dirichlet; dof[i].valor = 10 * i;
        dof[i * vertices_linha].bc = DegreeOfFreedom::Dirichlet; dof[i * vertices_linha].valor = 10 * i;
        dof[vertices_total - 1 - i].bc = DegreeOfFreedom::Dirichlet; dof[vertices_total - 1 - i].valor = 10 * i;
        dof[vertices_total - 1 - i * vertices_linha].bc = DegreeOfFreedom::Dirichlet; dof[vertices_total - 1 - i * vertices_linha].valor = 10 * i;
    }*/

    return dof;
}

TEST(FiniteElementsMethod, GlobalMatrix)
{
    std::vector<DegreeOfFreedom> dof = getDegreesOfFreedom();

    DenseMatrix<double> K(vertices_total, vertices_total);
    DenseMatrix<double> X(vertices_total, 1);
    DenseMatrix<double> F(vertices_total, 1);

    for (unsigned int i = 0; i < linhas_colunas; ++i)
    {
        for (unsigned int j = 0; j < linhas_colunas; ++j)
        {
            auto globalUp = [i, j](unsigned int v)
            {
                if (v == 0)
                    return (i + 0) * vertices_linha + j + 0;
                else if (v == 1)
                    return (i + 1) * vertices_linha + j + 0;
                else
                    return (i + 0) * vertices_linha + j + 1;
            };
            auto globalDn = [i, j](unsigned int v)
            {
                if (v == 0)
                    return (i + 1) * vertices_linha + j + 0;
                else if (v == 1)
                    return (i + 1) * vertices_linha + j + 1;
                else
                    return (i + 0) * vertices_linha + j + 1;
            };

            FiniteElement FeUp(std::vector<const DegreeOfFreedom*>
            {
                &dof[globalUp(0)],
                &dof[globalUp(1)],
                &dof[globalUp(2)]
            });

            DenseMatrix<double> Kup = FeUp.stiffness();
            for (unsigned int k = 0; k < Kup.rows(); ++k)
            {
                unsigned gK = globalUp(k);
                for (unsigned int l = 0; l < Kup.columns(); ++l)
                {
                    unsigned gL = globalUp(l);
                    K[gK][gL] += Kup[k][l];
                }
            }

            FiniteElement FeDn(std::vector<const DegreeOfFreedom*>
            {
                &dof[globalDn(0)],
                &dof[globalDn(1)],
                &dof[globalDn(2)]
            });

            DenseMatrix<double> Kdn = FeDn.stiffness();
            for (unsigned int k = 0; k < Kdn.rows(); ++k)
            {
                unsigned gK = globalDn(k);
                for (unsigned int l = 0; l < Kdn.columns(); ++l)
                {
                    unsigned gL = globalDn(l);
                    K[gK][gL] += Kdn[k][l];
                }
            }
        }
    }

    for (unsigned int i = 0; i < K.rows(); ++i)
    {
        if (dof[i].bc == DegreeOfFreedom::Dirichlet)
        {
            F[i][0] = dof[i].valor * K[i][i];
            for (unsigned int j = 0; j < K.columns(); ++j)
            {
                if (i == j) continue;
                if (dof[j].bc != DegreeOfFreedom::Dirichlet)
                {
                    F[j][0] += -(dof[i].valor) * K[j][i];
                }
                K[i][j] = K[j][i] = 0;
            }
        }
    }

    X = ConjugateGradientMethod::solve<DenseMatrix<double>>(K, F, 1e-5, 1000);

    /*std::ofstream of("saida_matrixglobal.csv");
    of << "X,Y,Potencial" << std::endl;
    for (unsigned int i = 0; i < vertices_linha; ++i)
    {
        for (unsigned int j = 0; j < vertices_linha; ++j)
        {
            of << i << "," << j << "," << X[i * vertices_linha + j][0] << std::endl;
        }
    }
    of.close();*/
}

TEST(FiniteElementsMethod, CompressedGlobalMatrix)
{
    std::vector<DegreeOfFreedom> dof = getDegreesOfFreedom();

    std::vector<std::tuple<size_t, size_t, double>> values;
    DenseMatrix<double> X(vertices_total, 1);
    DenseMatrix<double> F(vertices_total, 1);

    for (unsigned int i = 0; i < linhas_colunas; ++i)
    {
        for (unsigned int j = 0; j < linhas_colunas; ++j)
        {
            auto globalUp = [i, j](unsigned int v)
            {
                if (v == 0)
                    return (i + 0) * vertices_linha + j + 0;
                else if (v == 1)
                    return (i + 1) * vertices_linha + j + 0;
                else
                    return (i + 0) * vertices_linha + j + 1;
            };
            auto globalDn = [i, j](unsigned int v)
            {
                if (v == 0)
                    return (i + 1) * vertices_linha + j + 0;
                else if (v == 1)
                    return (i + 1) * vertices_linha + j + 1;
                else
                    return (i + 0) * vertices_linha + j + 1;
            };

            FiniteElement FeUp(std::vector<const DegreeOfFreedom*>
            {
                &dof[globalUp(0)],
                &dof[globalUp(1)],
                &dof[globalUp(2)]
            });

            FeUp.applyBoundaryConditions();
            DenseMatrix<double> Kup = FeUp.stiffness();
            for (unsigned int k = 0; k < Kup.rows(); ++k)
            {
                unsigned gK = globalUp(k);
                for (unsigned int l = 0; l < Kup.columns(); ++l)
                {
                    unsigned gL = globalUp(l);
                    values.push_back(std::make_tuple(gK, gL, Kup[k][l]));
                }
            }
            DenseMatrix<double> Fup = FeUp.constants();
            for (unsigned int k = 0; k < Kup.rows(); ++k)
            {
                unsigned gK = globalUp(k);
                F[gK][0] += Fup[k][0];
            }

            FiniteElement FeDn(std::vector<const DegreeOfFreedom*>
            {
                &dof[globalDn(0)],
                &dof[globalDn(1)],
                &dof[globalDn(2)]
            });

            FeDn.applyBoundaryConditions();
            DenseMatrix<double> Kdn = FeDn.stiffness();
            for (unsigned int k = 0; k < Kdn.rows(); ++k)
            {
                unsigned gK = globalDn(k);
                for (unsigned int l = 0; l < Kdn.columns(); ++l)
                {
                    unsigned gL = globalDn(l);
                    values.push_back(std::make_tuple(gK, gL, Kdn[k][l]));
                }
            }
            DenseMatrix<double> Fdn = FeDn.constants();
            for (unsigned int k = 0; k < Kup.rows(); ++k)
            {
                unsigned gK = globalDn(k);
                F[gK][0] += Fdn[k][0];
            }
        }
    }

    std::sort(values.begin(), values.end());
    auto * curr = &values.front();
    for (size_t i = 1; i < values.size(); ++i)
    {
        if (std::get<0>(values[i]) == std::get<0>(*curr) && std::get<1>(values[i]) == std::get<1>(*curr))
        {
            std::get<2>(*curr) += std::get<2>(values[i]);
            std::get<2>(values[i]) = 0.;
        }
        else
        {
            curr = &values[i];
        }
    }

    values.erase(std::remove_if(values.begin(), values.end(), [](std::tuple<size_t, size_t, double> t) {return std::get<2>(t) == 0.;}), values.end());

    CompressedMatrix<double> compressedK(vertices_total, vertices_total, values);
    values.clear();
    values.shrink_to_fit();

    X = ConjugateGradientMethod::solve<DenseMatrix<double>>(compressedK, F, 1e-5, 1000);

    /*std::ofstream of("saida_matrixglobalcomprimida.csv");
    of << "X,Y,Potencial" << std::endl;
    for (unsigned int i = 0; i < vertices_linha; ++i)
    {
        for (unsigned int j = 0; j < vertices_linha; ++j)
        {
            of << i << "," << j << "," << X[i * vertices_linha + j][0] << std::endl;
        }
    }
    of.close();*/
}

TEST(FiniteElementsMethod, ElementByElement)
{
    std::vector<DegreeOfFreedom> dof = getDegreesOfFreedom();

    DenseMatrix<double> X(vertices_total, 1);
    DenseMatrix<double> R(vertices_total, 1);
    DenseMatrix<double> P(vertices_total, 1);
    DenseMatrix<double> V(vertices_total, 1);
    std::vector<PointerMatrix<double>> Xe;
    std::vector<PointerMatrix<double>> Re;
    std::vector<PointerMatrix<double>> Pe;
    std::vector<PointerMatrix<double>> Ve;
    std::vector<FiniteElement> Fe;

    for (unsigned int i = 0; i < linhas_colunas; ++i)
    {
        for (unsigned int j = 0; j < linhas_colunas; ++j)
        {
            Fe.push_back(FiniteElement(std::vector<const DegreeOfFreedom*>
            {
                &dof[(i + 0) * vertices_linha + j + 0],
                &dof[(i + 1) * vertices_linha + j + 0],
                &dof[(i + 0) * vertices_linha + j + 1]
            }));

            PointerMatrix<double> upX(3, 1);
            upX[0][0] = &X[(i + 0) * vertices_linha + j + 0][0];
            upX[1][0] = &X[(i + 1) * vertices_linha + j + 0][0];
            upX[2][0] = &X[(i + 0) * vertices_linha + j + 1][0];
            Xe.push_back(upX);

            PointerMatrix<double> upR(3, 1);
            upR[0][0] = &R[(i + 0) * vertices_linha + j + 0][0];
            upR[1][0] = &R[(i + 1) * vertices_linha + j + 0][0];
            upR[2][0] = &R[(i + 0) * vertices_linha + j + 1][0];
            Re.push_back(upR);

            PointerMatrix<double> upP(3, 1);
            upP[0][0] = &P[(i + 0) * vertices_linha + j + 0][0];
            upP[1][0] = &P[(i + 1) * vertices_linha + j + 0][0];
            upP[2][0] = &P[(i + 0) * vertices_linha + j + 1][0];
            Pe.push_back(upP);

            PointerMatrix<double> upV(3, 1);
            upV[0][0] = &V[(i + 0) * vertices_linha + j + 0][0];
            upV[1][0] = &V[(i + 1) * vertices_linha + j + 0][0];
            upV[2][0] = &V[(i + 0) * vertices_linha + j + 1][0];
            Ve.push_back(upV);

            Fe.push_back(FiniteElement(std::vector<const DegreeOfFreedom*>
            {
                &dof[(i + 1) * vertices_linha + j + 0],
                &dof[(i + 1) * vertices_linha + j + 1],
                &dof[(i + 0) * vertices_linha + j + 1]
            }));

            PointerMatrix<double> dnX(3, 1);
            dnX[0][0] = &X[(i + 1) * vertices_linha + j + 0][0];
            dnX[1][0] = &X[(i + 1) * vertices_linha + j + 1][0];
            dnX[2][0] = &X[(i + 0) * vertices_linha + j + 1][0];
            Xe.push_back(dnX);

            PointerMatrix<double> dnR(3, 1);
            dnR[0][0] = &R[(i + 1) * vertices_linha + j + 0][0];
            dnR[1][0] = &R[(i + 1) * vertices_linha + j + 1][0];
            dnR[2][0] = &R[(i + 0) * vertices_linha + j + 1][0];
            Re.push_back(dnR);

            PointerMatrix<double> dnP(3, 1);
            dnP[0][0] = &P[(i + 1) * vertices_linha + j + 0][0];
            dnP[1][0] = &P[(i + 1) * vertices_linha + j + 1][0];
            dnP[2][0] = &P[(i + 0) * vertices_linha + j + 1][0];
            Pe.push_back(dnP);

            PointerMatrix<double> dnV(3, 1);
            dnV[0][0] = &V[(i + 1) * vertices_linha + j + 0][0];
            dnV[1][0] = &V[(i + 1) * vertices_linha + j + 1][0];
            dnV[2][0] = &V[(i + 0) * vertices_linha + j + 1][0];
            Ve.push_back(dnV);
        }
    }

    Xe.shrink_to_fit();
    Re.shrink_to_fit();
    Pe.shrink_to_fit();
    Ve.shrink_to_fit();
    Fe.shrink_to_fit();

    unsigned int maxIterations = 10000;
    double precision = 1e-5;

    for (unsigned int i = 0; i < Fe.size(); ++i)
    {
        Fe[i].applyBoundaryConditions();
        Re[i] = (DenseMatrix<double>)(Re[i] + (DenseMatrix<double>)(Fe[i].constants() - (DenseMatrix<double>)(Fe[i].stiffness() * Xe[i])));
    }
    double e = 1.;

    while(std::sqrt(e) > precision && maxIterations--)
    {
        double new_e = ((DenseMatrix<double>)(R.transpose() * R))[0][0];
        double beta = new_e / e;
        e = new_e;
        P = R + (DenseMatrix<double>)(beta * P);

        V = DenseMatrix<double>(vertices_total, 1);
        for (unsigned int i = 0; i < Fe.size(); ++i)
        {
            Ve[i] = (DenseMatrix<double>)(Ve[i] + (DenseMatrix<double>)(Fe[i].stiffness() * Pe[i]));
        }
        double alpha = e / ((DenseMatrix<double>)(P.transpose() * V))[0][0];
        X = X + (DenseMatrix<double>)(alpha * P);
        R = R - (DenseMatrix<double>)(alpha * V);
    }

    /*std::ofstream of("saida.csv");
    of << "X,Y,Potencial" << std::endl;
    for (unsigned int i = 0; i < vertices_linha; ++i)
    {
        for (unsigned int j = 0; j < vertices_linha; ++j)
        {
            of << i << "," << j << "," << X[i * vertices_linha + j][0] << std::endl;
        }
    }
    of.close();*/
}
