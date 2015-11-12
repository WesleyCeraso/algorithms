#ifndef POINTERMATRIXELEMENTACESSOR_H
#define POINTERMATRIXELEMENTACESSOR_H

#include "matrixelementacessor.h"
#include "pointermatrixelementproxy.h"

template <class> class PointerMatrix;

template <class T, bool Const>
class PointerMatrixElementAcessor : public MatrixElementAcessor<PointerMatrixElementAcessor<T, Const>>
{
    friend class MatrixElementAcessor<PointerMatrixElementAcessor>;
    friend class PointerMatrixElementAcessor<T, true>;

    typedef typename matrix_element_acessor_traits<PointerMatrixElementAcessor<T, Const>>::return_type return_type;
    typedef typename matrix_element_acessor_traits<PointerMatrixElementAcessor<T, Const>>::size_type size_type;
    typedef typename matrix_element_acessor_traits<PointerMatrixElementAcessor<T, Const>>::value_type value_type;

public:
    PointerMatrixElementAcessor(PointerMatrix<T>& matrix, const size_type row):
        MatrixElementAcessor<PointerMatrixElementAcessor<T, Const>>(row),
        m_matrix(matrix)
    {}

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    PointerMatrixElementAcessor(PointerMatrixElementAcessor<T, C>&& rhs):
        MatrixElementAcessor<PointerMatrixElementAcessor<T, Const>>(std::move(rhs)),
        m_matrix(rhs.m_matrix)
    {}

protected:
    return_type dereference(const size_type row, const size_type column) const
    {
        return return_type(&m_matrix.m_values[row * m_matrix.columns() + column]);
    }

private:
    PointerMatrixElementAcessor(const PointerMatrixElementAcessor&) {}
    PointerMatrixElementAcessor& operator=(const PointerMatrixElementAcessor&) {return *this;}
    PointerMatrixElementAcessor& operator=(PointerMatrixElementAcessor&&) {return *this;}

private:
    PointerMatrix<T>& m_matrix;
};

template <class T, bool Const>
struct matrix_element_acessor_traits<PointerMatrixElementAcessor<T, Const>>
{
    typedef typename PointerMatrix<T>::value_type value_type;
    typedef typename PointerMatrix<T>::size_type size_type;
    typedef typename std::conditional<Const, const PointerMatrixItemProxy<T>, PointerMatrixItemProxy<T>>::type return_type;
};

#endif

