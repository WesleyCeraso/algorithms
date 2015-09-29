#ifndef SUBSTITUTIONMETHOD_H
#define SUBSTITUTIONMETHOD_H

#include <type_traits>
#include <cassert>

class SubstitutionMethod
{
public:
    template <class T, class D>
    static D forward(const T& coeficients, const D& constants)
    {
        assert(coeficients.rows() == coeficients.columns() && coeficients.rows() == constants.rows());
        D variables(coeficients.rows(), 1);

        for(typename T::size_type i = 0; i < coeficients.rows(); ++i)
        {
            variables[i][0] = constants[i][0];

            for(typename T::size_type j = 0; j < i; ++j)
            {
                variables[i][0] -= coeficients[i][j] * variables[j][0];
            }

            variables[i][0] /= coeficients[i][i];
        }

        return variables;
    }

    template <class T, class D>
    static D backward(const T& coeficients, const D& constants)
    {
        assert(coeficients.rows() == coeficients.columns() && coeficients.rows() == constants.rows());
        D variables(coeficients.rows(), 1);

        for(typename T::size_type i = coeficients.rows() - 1; i < coeficients.rows(); --i)
        {
            variables[i][0] = constants[i][0];

            for(typename T::size_type j = i + 1; j < coeficients.rows(); ++j)
            {
                variables[i][0] -= coeficients[i][j] * variables[j][0];
            }

            variables[i][0] /= coeficients[i][i];
        }

        return variables;
    }
};

#endif
