#ifndef STABILIZEDBICONJUGATEGRADIENTMETHOD_H
#define STABILIZEDBICONJUGATEGRADIENTMETHOD_H

#include "../matrices/matrixutils.h"
#include <type_traits>
#include <cassert>

class StabilizedBiConjugateGradientMethod
{
public:
    template <class R, class T, class D>
    static R solve(const T& A, const D& constants, typename R::value_type precision, size_t maxIterations = 10)
    {
        typedef typename R::value_type common_type;
        assert(A.rows() == A.columns() && A.rows() == constants.rows());

        R x(A.rows(), 1);
        R r = constants - (R)(A * x);
        R rOriginalTranspose = r.transpose();
        R p = R(A.rows(), 1);
        R v = R(A.rows(), 1);

        common_type alpha = 1.;
        common_type omega = 1.;
        common_type e = ((R)(rOriginalTranspose * r))[0][0];
        common_type old_e = 1.;

        while(std::sqrt(e) > precision && maxIterations--)
        {
            common_type beta = (e / old_e) * (alpha / omega);
            p = r + (R)(beta * (R)(p - (R)(omega * v)));
            v = A * p;
            alpha = e / ((R)(rOriginalTranspose * v))[0][0];
            R s = r - (R)(alpha * v);
            R t = A * s;
            R tTransposed = t.transpose();
            omega = ((R)(tTransposed * s))[0][0] / std::pow(MatrixUtils::euclideanNorm(tTransposed), 2);
            old_e = e;
            e = -((R)(omega * (R)(rOriginalTranspose * t)))[0][0];
            x = x + (R)((R)(alpha * p) + (R)(omega * s));
            r = s - (R)(omega * t);
        }

        return x;
    }
};

#endif
