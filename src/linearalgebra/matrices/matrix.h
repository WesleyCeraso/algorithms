#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include "matrixmultiplication.h"
#include "matrixsummation.h"
#include "matrixsubtraction.h"
#include "matrixiterator.h"

template <class T>
struct matrix_traits
{
    static const bool is_matrix = false;
};

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

    typedef typename matrix_traits<T>::acessor acessor;
    typedef typename matrix_traits<T>::const_acessor const_acessor;

public:
    size_type rows() const {return m_rows;}
    size_type columns() const {return m_columns;}

    void clear() {static_cast<T*>(this)->doClear();}
    T transpose() const {return static_cast<const T*>(this)->doTranspose();}

    acessor operator[](size_type index) {return acessor(*static_cast<T*>(this), index);}
    const_acessor operator[](size_type index) const {return const_cast<Matrix*>(this)->operator[](index);}

    iterator begin(const size_type row = size_type()) {return iterator(*static_cast<T*>(this), row);}
    const_iterator begin(const size_type row = size_type()) const {return const_cast<Matrix*>(this)->begin(row);}
    const_iterator cbegin(const size_type row = size_type()) const {return const_cast<Matrix*>(this)->begin(row);}

    iterator end() {return iterator(*static_cast<T*>(this), rows());}
    const_iterator end() const {return const_cast<Matrix*>(this)->end();}
    const_iterator cend() const {return const_cast<Matrix*>(this)->end();}

    template <class TT>
    bool operator==(const Matrix<TT>& rhs) const;
    template <class TT>
    bool operator!=(const Matrix<TT>& rhs) const;

protected:
    Matrix(size_type rows, size_type columns);

    Matrix(const Matrix& rhs);
    Matrix& operator=(const Matrix& rhs);

    Matrix(Matrix&& rhs);
    Matrix& operator=(Matrix&& rhs);

    template <class TT>
    Matrix(const Matrix<TT>& rhs);
    template <class TT>
    Matrix& operator=(const Matrix<TT>& rhs);

private:
    size_type m_rows;
    size_type m_columns;
};

template <class T>
template <class TT>
bool Matrix<T>::operator==(const Matrix<TT>& rhs) const
{
    const Matrix& lhs = *this;
    assert(lhs.rows() == rhs.rows() && lhs.columns() == rhs.columns());

    for(size_type i = size_type(); i < rhs.rows(); ++i)
    {
        for(size_type j = size_type(); j < rhs.columns(); ++j)
        {
            if (lhs[i][j] != rhs[i][j])
                return false;
        }
    }

    return true;
}

template <class T>
template <class TT>
bool Matrix<T>::operator!=(const Matrix<TT>& rhs) const
{
    return !this->operator ==(rhs);
}

template <class T>
std::ostream& operator<<(std::ostream& of, const Matrix<T>& rhs)
{
    typedef typename Matrix<T>::size_type size_type;

    std::streamsize width = std::max(of.width(), std::streamsize(2));
    std::streamsize precision = of.precision();

    for(size_type i = size_type(); i < rhs.rows(); ++i)
    {
        of.width(0);
        of << "|";
        for(size_type j = size_type(); j < rhs.columns(); ++j)
        {
            of.width(width);
            of.precision(precision);
            of << rhs[i][j];
        }

        of.width(width);
        of << "|" << std::endl;
    }

    return of;
}

template <class T>
Matrix<T>::Matrix(size_type rows, size_type columns):
    m_rows(rows),
    m_columns(columns)
{
    assert(rows && columns);
}

template <class T>
Matrix<T>::Matrix(const Matrix& rhs):
    m_rows(rhs.m_rows),
    m_columns(rhs.m_columns)
{}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix& rhs)
{
    assert(m_rows == rhs.m_rows && m_columns == rhs.m_columns);
    return *this;
}

template <class T>
Matrix<T>::Matrix(Matrix&& rhs):
    m_rows(std::move(rhs.m_rows)),
    m_columns(std::move(rhs.m_columns))
{}

template <class T>
Matrix<T>& Matrix<T>::operator=(Matrix&& rhs)
{
    assert(m_rows == rhs.m_rows && m_columns == rhs.m_columns);
    return *this;
}

template <class T>
template <class TT>
Matrix<T>::Matrix(const Matrix<TT>& rhs):
    m_rows(rhs.rows()),
    m_columns(rhs.columns())
{}

template <class T>
template <class TT>
Matrix<T>& Matrix<T>::operator=(const Matrix<TT>& rhs)
{
    assert(m_rows == rhs.rows() && m_columns == rhs.columns());
    return *this;
}

#endif
