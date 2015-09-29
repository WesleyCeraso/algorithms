#ifndef CONJUGATEGRADIENTMETHOD_H
#define CONJUGATEGRADIENTMETHOD_H

#include <type_traits>
#include <cassert>
#include <cmath>

class ConjugateGradientMethod
{
public:
    template <class R, class T, class D>
    static R solve(const T& A, const D& constants, typename R::value_type precision, size_t maxIterations = 10)
    {
        typedef typename R::value_type common_type;
        assert(A.rows() == A.columns() && A.rows() == constants.rows());

        R x(A.rows(), 1);
        R r = constants - (R)(A * x);
        R p = R(A.rows(), 1);
        common_type e = 1.;

        while(std::sqrt(e) > precision && maxIterations--)
        {
            common_type new_e = ((R)(r.transpose() * r))[0][0];
            common_type beta = new_e / e;
            e = new_e;
            p = r + (R)(beta * p);

            R v = A * p;
            common_type alpha = e / ((R)(p.transpose() * v))[0][0];
            x = x + (R)(alpha * p);
            r = r - (R)(alpha * v);
        }

        return x;
    }
};

#endif
