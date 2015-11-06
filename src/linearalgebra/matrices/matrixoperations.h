#ifndef MATRIXOPERATIONS_H
#define MATRIXOPERATIONS_H
#include "matrixmultiplylhshandler.h"
#include "matrixmultiplyrhshandler.h"
#include <type_traits>

template <class C>
class Matrix;

template <class R>
class MatrixOperations {
public:
    template <class T, class TT>
    static R multiply(const Matrix<T>& lhs, const Matrix<TT>& rhs);
    template <class T, class D>
    static R multiply(const Matrix<T>& lhs, const D rhs);
    template <class T, class D>
    static R multiply(const D lhs, const Matrix<T>& rhs);

    template <class T, class TT>
    static R sum(const Matrix<T>& lhs, const Matrix<TT>& rhs);
    template <class T, class D>
    static R sum(const Matrix<T>& lhs, const D rhs);
    template <class T, class D>
    static R sum(const D lhs, const Matrix<T>& rhs);

    template <class T, class TT>
    static R subtract(const Matrix<T>& lhs, const Matrix<TT>& rhs);
    template <class T, class D>
    static R subtract(const Matrix<T>& lhs, const D rhs);
    template <class T, class D>
    static R subtract(const D lhs, const Matrix<T>& rhs);
};

template <class R>
template <class T, class TT>
R MatrixOperations<R>::multiply(const Matrix<T>& lhs, const Matrix<TT>& rhs)
{
    assert(lhs.columns() == rhs.rows());
    R result(lhs.rows(), rhs.columns());

    MatrixMultiplyLhsHandler<T> lhsH(*static_cast<const T*>(&lhs));
    MatrixMultiplyRhsHandler<TT> rhsH(*static_cast<const TT*>(&rhs));
    for (;lhsH.row() < lhs.rows(); lhsH.nextRowFirstColumn(), rhsH.resetAll())
    {
        for (;rhsH.column() < rhs.columns(); lhsH.resetColumn(), rhsH.nextColumnFirstRow())
        {
            typename R::value_type sum = typename R::value_type();
            while (!lhsH.endRow() && !rhsH.endColumn())
            {
                if (lhsH.column() == rhsH.row())
                {
                    sum += (lhsH.value()) * (rhsH.value());
                    lhsH.nextColumnSameRow();
                    rhsH.nextRowSameColumn();
                }
                else if (lhsH.column() < rhsH.row())
                {
                    lhsH.nextColumnSameRow();
                }
                else if (lhsH.column() > rhsH.row())
                {
                    rhsH.nextRowSameColumn();
                }
            }
            if (sum) result[lhsH.row()][rhsH.column()] = sum;
        }
    }

    return result;
}

template <class R>
template <class T, class D>
R MatrixOperations<R>::multiply(const Matrix<T>& lhs, const D rhs)
{
    R result(lhs.rows(), lhs.columns());

    if(rhs == D())
    {
        return result;
    }

    typename Matrix<T>::const_iterator itL = lhs.beginNonZero();
    typename Matrix<T>::const_iterator endL = lhs.endNonZero();

    while(itL != endL)
    {
        result[itL.row()][itL.column()] = *itL * rhs;
        itL.nextNonZero();
    }

    return result;
}

template <class R>
template <class T, class D>
R MatrixOperations<R>::multiply(const D lhs, const Matrix<T>& rhs)
{
    return rhs * lhs;
}

template <class R>
template <class T, class TT>
R MatrixOperations<R>::sum(const Matrix<T>& lhs, const Matrix<TT>& rhs)
{
    assert(lhs.rows() == rhs.rows() && lhs.columns() == rhs.columns());
    R result(lhs.rows(), rhs.columns());
    typename Matrix<T>::const_iterator itL = lhs.beginNonZero();
    typename Matrix<T>::const_iterator endL = lhs.endNonZero();
    typename Matrix<TT>::const_iterator itR = rhs.beginNonZero();
    typename Matrix<TT>::const_iterator endR = rhs.endNonZero();

    while(itL != endL && itR != endR)
    {
        if(itL.row() < itR.row() || (itL.row() == itR.row() && itL.column() < itR.column()))
        {
            result[itL.row()][itL.column()] = *itL;
            itL.nextNonZero();
        }
        else if(itL.row() > itR.row() || (itL.row() == itR.row() && itL.column() > itR.column()))
        {
            result[itR.row()][itR.column()] = *itR;
            itR.nextNonZero();
        }
        else
        {
            typename R::value_type sum = *itL + *itR;

            if(sum)
            {
                result[itL.row()][itL.column()] = sum;
            }

            itL.nextNonZero();
            itR.nextNonZero();
        }
    }

    while(itL != endL)
    {
        result[itL.row()][itL.column()] = *itL;
        itL.nextNonZero();
    }

    while(itR != endR)
    {
        result[itR.row()][itR.column()] = *itR;
        itR.nextNonZero();
    }

    return result;
}

template <class R>
template <class T, class D>
R MatrixOperations<R>::sum(const Matrix<T>& lhs, const D rhs)
{
    R result(lhs.rows(), lhs.columns(), rhs);
    typename Matrix<T>::const_iterator itL = lhs.beginNonZero();
    typename Matrix<T>::const_iterator endL = lhs.endNonZero();

    while(itL != endL)
    {
        result[itL.row()][itL.column()] += *itL;
        itL.nextNonZero();
    }

    return result;
}

template <class R>
template <class T, class D>
R MatrixOperations<R>::sum(const D lhs, const Matrix<T>& rhs)
{
    return rhs + lhs;
}

template <class R>
template <class T, class TT>
R MatrixOperations<R>::subtract(const Matrix<T>& lhs, const Matrix<TT>& rhs)
{
    assert(lhs.rows() == rhs.rows() && lhs.columns() == rhs.columns());
    R result(lhs.rows(), rhs.columns());
    typename Matrix<T>::const_iterator itL = lhs.beginNonZero();
    typename Matrix<T>::const_iterator endL = lhs.endNonZero();
    typename Matrix<TT>::const_iterator itR = rhs.beginNonZero();
    typename Matrix<TT>::const_iterator endR = rhs.endNonZero();

    while(itL != endL && itR != endR)
    {
        if(itL.row() < itR.row() || (itL.row() == itR.row() && itL.column() < itR.column()))
        {
            result[itL.row()][itL.column()] = *itL;
            itL.nextNonZero();
        }
        else if(itL.row() > itR.row() || (itL.row() == itR.row() && itL.column() > itR.column()))
        {
            result[itR.row()][itR.column()] = -*itR;
            itR.nextNonZero();
        }
        else
        {
            typename R::value_type sum = *itL - *itR;

            if(sum)
            {
                result[itL.row()][itL.column()] = sum;
            }

            itL.nextNonZero();
            itR.nextNonZero();
        }
    }

    while(itL != endL)
    {
        result[itL.row()][itL.column()] = *itL;
        itL.nextNonZero();
    }

    while(itR != endR)
    {
        result[itR.row()][itR.column()] = -*itR;
        itR.nextNonZero();
    }

    return result;
}

template <class R>
template <class T, class D>
R MatrixOperations<R>::subtract(const Matrix<T>& lhs, const D rhs)
{
    return lhs + (-rhs);
}

template <class R>
template <class T, class D>
R MatrixOperations<R>::subtract(const D lhs, const Matrix<T>& rhs)
{
    R result(rhs.rows(), rhs.columns());
    typename Matrix<T>::const_iterator itR = rhs.beginNonZero();
    typename Matrix<T>::const_iterator endR = rhs.endNonZero();

    while(itR != endR)
    {
        result[itR.row()][itR.column()] = lhs - *itR;
        itR.nextNonZero();
    }

    return result;
}

#endif
