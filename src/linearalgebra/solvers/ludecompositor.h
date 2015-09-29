#ifndef LUDECOMPOSITOR_H
#define LUDECOMPOSITOR_H

#include "../matrices/matrixutils.h"

class LuDecompositor
{
public:
    template <class L, class U = L, class T>
    static std::pair<L, L> decompose(const T& coeficients)
    {
        assert(coeficients.rows() == coeficients.columns());
        L l(MatrixUtils::identity<L>(coeficients.rows()));
        U u(coeficients.rows(), coeficients.columns());
        typename T::const_iterator cit = coeficients.cbegin();
        typename T::const_iterator end = coeficients.cend();

        while(cit != end)
        {
            if(cit.row() > cit.column())
            {
                //L
                typename L::value_type sum = typename L::value_type();

                for(typename L::size_type k = 0; k < cit.column(); ++k)
                {
                    sum += l[cit.row()][k] * u[k][cit.column()];
                }

                l[cit.row()][cit.column()] = (1. / u[cit.column()][cit.column()]) * (*cit - sum);
            }
            else
            {
                //U
                typename U::value_type sum = typename U::value_type();

                for(typename U::size_type k = 0; k < cit.row(); ++k)
                {
                    sum += l[cit.row()][k] * u[k][cit.column()];
                }

                u[cit.row()][cit.column()] = *cit - sum;
            }

            ++cit;
        }

        return std::make_pair(l, u);
    }
};

#endif
