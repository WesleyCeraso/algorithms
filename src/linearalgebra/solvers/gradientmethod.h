#ifndef GRADIENTMETHOD_H
#define GRADIENTMETHOD_H

#include <type_traits>
#include <cassert>
#include <cmath>

class GradientMethod
{
public:
    template <class R, class T, class D>
    static R solve(const T& A, const D& constants, typename R::value_type precision, size_t maxIterations = 10)
    {
        typedef typename R::value_type common_type;
        assert(A.rows() == A.columns() && A.rows() == constants.rows());

        R x(A.rows(), 1);
        R r = constants - (R)(A * x);
        R rTranspose = r.transpose();
        common_type e = ((R)(r.transpose() * r))[0][0];

        while(std::sqrt(e) > precision && maxIterations--)
        {
            common_type s = e / ((R)((R)(rTranspose * A) * r))[0][0];
            x = x + (R)(s * r);
            r = constants - (R)(A * x);
            rTranspose = r.transpose();
            e = ((R)(rTranspose * r))[0][0];
        }

        return x;
    }
};

#endif
