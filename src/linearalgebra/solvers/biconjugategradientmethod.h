#ifndef BICONJUGATEGRADIENTMETHOD_H
#define BICONJUGATEGRADIENTMETHOD_H

#include <type_traits>
#include <cassert>
#include <cmath>

class BiConjugateGradientMethod
{
public:
    template <class R, class T, class D>
    static R solve(const T& A, const D& constants, typename R::value_type precision, size_t maxIterations = 10)
    {
        typedef typename R::value_type common_type;
        assert(A.rows() == A.columns() && A.rows() == constants.rows());

        T At = A.transpose();
        R x(A.rows(), 1);
        R r = constants - (R)(A * x);
        R r2 = r;
        R p = R(A.rows(), 1);
        R p2 = p;
        common_type e = 1.;

        while(std::sqrt(e) > precision && maxIterations--)
        {
            common_type new_e = ((R)(r2.transpose() * r))[0][0];
            common_type beta = new_e / e;
            e = new_e;
            p = r + (R)(beta * p);
            p2 = r2 + (R)(beta * p2);

            R v = A * p;
            common_type alpha = e / ((R)(p2.transpose() * v))[0][0];
            x = x + (R)(alpha * p);
            r = r - (R)(alpha * v);
            r2 = r2 - (R)(alpha * (R)(At * p2));
        }

        return x;
    }
};

#endif
