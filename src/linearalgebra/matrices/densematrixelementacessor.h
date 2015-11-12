#ifndef DENSEMATRIXELEMENTACESSOR_H
#define DENSEMATRIXELEMENTACESSOR_H

#include "matrixelementacessor.h"

template <class> class DenseMatrix;

template <class T, bool Const>
class DenseMatrixElementAcessor : public MatrixElementAcessor<DenseMatrixElementAcessor<T, Const>>
{
    friend class MatrixElementAcessor<DenseMatrixElementAcessor>;
    friend class DenseMatrixElementAcessor<T, true>;

    typedef typename matrix_element_acessor_traits<DenseMatrixElementAcessor<T, Const>>::return_type return_type;
    typedef typename matrix_element_acessor_traits<DenseMatrixElementAcessor<T, Const>>::size_type size_type;
    typedef typename matrix_element_acessor_traits<DenseMatrixElementAcessor<T, Const>>::value_type value_type;

public:
    DenseMatrixElementAcessor(DenseMatrix<T>& matrix, const size_type row):
        MatrixElementAcessor<DenseMatrixElementAcessor<T, Const>>(row),
        m_matrix(matrix)
    {}

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    DenseMatrixElementAcessor(DenseMatrixElementAcessor<T, C>&& rhs):
        MatrixElementAcessor<DenseMatrixElementAcessor<T, Const>>(std::move(rhs)),
        m_matrix(rhs.m_matrix)
    {}

protected:
    return_type dereference(const size_type row, const size_type column) const
    {
        return m_matrix.m_values[row * m_matrix.columns() + column];
    }

private:
    DenseMatrixElementAcessor(const DenseMatrixElementAcessor&) {}
    DenseMatrixElementAcessor& operator=(const DenseMatrixElementAcessor&) {return *this;}
    DenseMatrixElementAcessor& operator=(DenseMatrixElementAcessor&&) {return *this;}

private:
    DenseMatrix<T>& m_matrix;
};

template <class T, bool Const>
struct matrix_element_acessor_traits<DenseMatrixElementAcessor<T, Const>>
{
    typedef typename DenseMatrix<T>::value_type value_type;
    typedef typename DenseMatrix<T>::size_type size_type;
    typedef typename std::conditional<Const, typename std::add_const<value_type>::type, typename std::add_lvalue_reference<value_type>::type>::type return_type;
};

#endif

