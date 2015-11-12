#ifndef MATRIXSUBTRACTION_H
#define MATRIXSUBTRACTION_H

#include <algorithm>

template <class> class Matrix;
template <class> struct matrix_traits;

template <class R, class T, class D, typename std::enable_if<matrix_traits<T>::is_matrix && matrix_traits<D>::is_matrix>::type* = nullptr>
R subtract(const T& lhs, const D& rhs);

template <class R, class T, class D, typename std::enable_if<matrix_traits<T>::is_matrix && !matrix_traits<D>::is_matrix>::type* = nullptr>
R subtract(const T& lhs, const D rhs);

template <class R, class T, class D, typename std::enable_if<!matrix_traits<T>::is_matrix && matrix_traits<D>::is_matrix>::type* = nullptr>
R subtract(const T lhs, const D& rhs);

template <class M, class N>
class MatrixSubtraction {
public:
    MatrixSubtraction(const M& lhs, const N& rhs):
        m_lhs(lhs),
        m_rhs(rhs)
    {}

    template <class R>
    operator R()
    {
        return subtract<R>(m_lhs, m_rhs);
    }

private:
    const M& m_lhs;
    const N& m_rhs;
};

template <class T, class D, typename std::enable_if<matrix_traits<T>::is_matrix || matrix_traits<D>::is_matrix>::type* = nullptr>
MatrixSubtraction<T, D> operator-(const T& lhs, const D& rhs)
{
    return MatrixSubtraction<T, D>(lhs, rhs);
}

template <class R, class T, class D, typename std::enable_if<matrix_traits<T>::is_matrix && matrix_traits<D>::is_matrix>::type*>
R subtract(const T& lhs, const D& rhs)
{
    assert(lhs.rows() == rhs.rows() && lhs.columns() == rhs.columns());
    R result(lhs.rows(), rhs.columns());
    typename T::const_iterator itL = lhs.begin();
    typename T::const_iterator endL = lhs.end();
    typename D::const_iterator itR = rhs.begin();
    typename D::const_iterator endR = rhs.end();

    while(itL != endL && itR != endR)
    {
        if(itL.row() < itR.row() || (itL.row() == itR.row() && itL.column() < itR.column()))
        {
            result[itL.row()][itL.column()] = *itL;
            ++itL;
        }
        else if(itL.row() > itR.row() || (itL.row() == itR.row() && itL.column() > itR.column()))
        {
            result[itR.row()][itR.column()] = -*itR;
            ++itR;
        }
        else
        {
            typename R::value_type sum = *itL - *itR;

            if(sum)
            {
                result[itL.row()][itL.column()] = sum;
            }

            ++itL;
            ++itR;
        }
    }

    while(itL != endL)
    {
        result[itL.row()][itL.column()] = *itL;
        ++itL;
    }

    while(itR != endR)
    {
        result[itR.row()][itR.column()] = -*itR;
        ++itR;
    }

    return result;
}

template <class R, class T, class D, typename std::enable_if<matrix_traits<T>::is_matrix && !matrix_traits<D>::is_matrix>::type*>
R subtract(const T& lhs, const D rhs)
{
    return lhs + (-rhs);
}

template <class R, class T, class D, typename std::enable_if<!matrix_traits<T>::is_matrix && matrix_traits<D>::is_matrix>::type*>
R subtract(const T lhs, const D& rhs)
{
    R result(rhs.rows(), rhs.columns());
    typename D::const_iterator itR = rhs.begin();
    typename D::const_iterator endR = rhs.end();

    while(itR != endR)
    {
        result[itR.row()][itR.column()] = lhs - *itR;
        ++itR;
    }

    return result;
}

#endif
