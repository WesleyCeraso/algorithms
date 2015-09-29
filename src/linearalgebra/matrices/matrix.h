#ifndef MATRIX_H
#define MATRIX_H

#include "matrixmultiplication.h"
#include "matrixsummation.h"
#include "matrixsubtraction.h"
#include "matrixiterator.h"

template <class T>
struct matrix_traits
{};

template <class T>
class Matrix
{
public:
    typedef typename matrix_traits<T>::value_type value_type;
    typedef typename matrix_traits<T>::size_type size_type;
    typedef typename matrix_traits<T>::difference_type difference_type;
    typedef typename matrix_traits<T>::reference reference;
    typedef typename matrix_traits<T>::const_reference const_reference;
    typedef typename matrix_traits<T>::pointer pointer;
    typedef typename matrix_traits<T>::const_pointer const_pointer;

    typedef typename matrix_traits<T>::iterator iterator;
    typedef typename matrix_traits<T>::const_iterator const_iterator;

public:
    size_type rows() const {return m_rows;}
    size_type columns() const {return m_columns;}

    void clear() {static_cast<T*>(this)->doClear();}
    T transpose() const {return static_cast<const T*>(this)->doTranspose();}

    iterator operator[](size_type index) {return iterator(static_cast<T*>(this), index, 0);}
    const_iterator operator[](size_type index) const {return const_cast<Matrix*>(this)->operator[](index);}

    iterator begin() {return iterator(static_cast<T*>(this), 0, 0);}
    const_iterator begin() const {return const_cast<Matrix*>(this)->begin();}
    const_iterator cbegin() const {return const_cast<Matrix*>(this)->begin();}

    iterator end() {return iterator(static_cast<T*>(this), rows(), 0);}
    const_iterator end() const {return const_cast<Matrix*>(this)->end();}
    const_iterator cend() const {return const_cast<Matrix*>(this)->end();}

    iterator beginNonZero() {iterator it = begin(); return it != end() && *it ? it : it.nextNonZero();}
    const_iterator beginNonZero() const {return const_cast<Matrix*>(this)->beginNonZero();}
    const_iterator cbeginNonZero() const {return const_cast<Matrix*>(this)->beginNonZero();}

    iterator endNonZero() {return end();}
    const_iterator endNonZero() const {return const_cast<Matrix*>(this)->endNonZero();}
    const_iterator cendNonZero() const {return const_cast<Matrix*>(this)->endNonZero();}

    template <class TT>
    bool operator==(const Matrix<TT>& rhs) const
    {
        const Matrix& lhs = *this;
        assert(lhs.rows() == rhs.rows() && lhs.columns() == rhs.columns());

        typename Matrix::const_iterator itL = lhs.beginNonZero();
        typename Matrix::const_iterator endL = lhs.endNonZero();
        typename Matrix<TT>::const_iterator itR = rhs.beginNonZero();
        typename Matrix<TT>::const_iterator endR = rhs.endNonZero();

        while(itL != endL && itR != endR)
        {
            if(*itL != *itR || itL.row() != itR.row() || itL.column() != itR.column())
            {
                return false;
            }

            itL.nextNonZero();
            itR.nextNonZero();
        }

        return itL == endL && itR == endR;
    }

    template <class TT>
    bool operator!=(const Matrix<TT>& rhs) const
    {
        return !this->operator ==(rhs);
    }

    friend std::ostream& operator<<(std::ostream& of, const Matrix& rhs)
    {
        std::streamsize width = of.width();
        std::streamsize precision = of.precision();

        for(size_type i = 0; i < rhs.rows(); ++i)
        {
            for(size_type j = 0; j < rhs.columns(); ++j)
            {
                of.width(width);
                of.precision(precision);
                of << rhs[i][j];
            }

            of << std::endl;
        }

        return of;
    }

protected:
    Matrix(size_type rows, size_type columns): m_rows(rows), m_columns(columns) {}
    Matrix(const Matrix& rhs): m_rows(rhs.rows()), m_columns(rhs.columns()) {}
    template <class TT>
    Matrix(const Matrix<TT>& rhs): m_rows(rhs.rows()), m_columns(rhs.columns()) {}

private:
    size_type m_rows;
    size_type m_columns;
};

// Multiplication -------------------------------------------------------------
template <class T, class TT>
MatrixMultiplication<Matrix<T>, Matrix<TT>> operator*(const Matrix<T>& lhs, const Matrix<TT>& rhs)
{
    return MatrixMultiplication<Matrix<T>, Matrix<TT>>(lhs, rhs);
}
template <class T, class D, typename std::enable_if<std::is_arithmetic<D>::value, int>::type = 0>
MatrixMultiplication<Matrix<T>, D> operator*(const Matrix<T>& lhs, const D& rhs)
{
    return MatrixMultiplication<Matrix<T>, D>(lhs, rhs);
}
template <class T, class D, typename std::enable_if<std::is_arithmetic<D>::value, int>::type = 0>
MatrixMultiplication<D, Matrix<T>> operator*(const D& lhs, const Matrix<T>& rhs)
{
    return MatrixMultiplication<D, Matrix<T>>(lhs, rhs);
}

//Sum -------------------------------------------------------------------------
template <class T, class TT>
MatrixSummation<Matrix<T>, Matrix<TT>> operator+(const Matrix<T>& lhs, const Matrix<TT>& rhs)
{
    return MatrixSummation<Matrix<T>, Matrix<TT>>(lhs, rhs);
}
template <class T, class D, typename std::enable_if<std::is_arithmetic<D>::value, int>::type = 0>
MatrixSummation<Matrix<T>, D> operator+(const Matrix<T>& lhs, const D& rhs)
{
    return MatrixSummation<Matrix<T>, D>(lhs, rhs);
}
template <class T, class D, typename std::enable_if<std::is_arithmetic<D>::value, int>::type = 0>
MatrixSummation<D, Matrix<T>> operator+(const D& lhs, const Matrix<T>& rhs)
{
    return MatrixSummation<D, Matrix<T>>(lhs, rhs);
}

//Subtraction -------------------------------------------------------------------------
template <class T, class TT>
MatrixSubtraction<Matrix<T>, Matrix<TT>> operator-(const Matrix<T>& lhs, const Matrix<TT>& rhs)
{
    return MatrixSubtraction<Matrix<T>, Matrix<TT>>(lhs, rhs);
}
template <class T, class D, typename std::enable_if<std::is_arithmetic<D>::value, int>::type = 0>
MatrixSubtraction<Matrix<T>, D> operator-(const Matrix<T>& lhs, const D& rhs)
{
    return MatrixSubtraction<Matrix<T>, D>(lhs, rhs);
}
template <class T, class D, typename std::enable_if<std::is_arithmetic<D>::value, int>::type = 0>
MatrixSubtraction<D, Matrix<T>> operator-(const D& lhs, const Matrix<T>& rhs)
{
    return MatrixSubtraction<D, Matrix<T>>(lhs, rhs);
}

#endif
