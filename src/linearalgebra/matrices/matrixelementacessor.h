#ifndef MATRIXELEMENTACESSOR_H
#define MATRIXELEMENTACESSOR_H

#include "matrix.h"

template <class I>
struct matrix_element_acessor_traits
{};

template <class I>
class MatrixElementAcessor
{
    template<class> friend class MatrixElementAcessor;
    typedef typename matrix_element_acessor_traits<I>::return_type return_type;
    typedef typename matrix_element_acessor_traits<I>::size_type size_type;

public:
    return_type operator[](const size_type column) const {return static_cast<const I*>(this)->dereference(m_row, column);}

protected:
    MatrixElementAcessor(const size_type row):
        m_row(row)
    {}

    template <class II>
    MatrixElementAcessor(MatrixElementAcessor<II>&& rhs):
        m_row(rhs.m_row)
    {}

private:
    MatrixElementAcessor(const MatrixElementAcessor&) {}
    MatrixElementAcessor& operator=(const MatrixElementAcessor&) {return *this;}
    MatrixElementAcessor& operator=(MatrixElementAcessor&&) {return *this;}

private:
    const size_type m_row;
};

#endif

