#ifndef DENSEMATRIXITERATOR_H
#define DENSEMATRIXITERATOR_H

#include <vector>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include "matrixiterator.h"

template <class> class DenseMatrix;

template <class T, bool Const>
class DenseMatrixIterator : public MatrixIterator<DenseMatrixIterator<T, Const>>
{
    friend class MatrixIterator<DenseMatrixIterator>;
    friend class DenseMatrixIterator<T, true>;

public:
    typedef typename matrix_iterator_traits<DenseMatrixIterator>::value_type value_type;
    typedef typename matrix_iterator_traits<DenseMatrixIterator>::size_type size_type;
    typedef typename matrix_iterator_traits<DenseMatrixIterator>::difference_type difference_type;
    typedef typename matrix_iterator_traits<DenseMatrixIterator>::return_type return_type;

public:
    DenseMatrixIterator(DenseMatrix<T>* matrix, const size_type row, const size_type column);

    DenseMatrixIterator(const DenseMatrixIterator<T, false>& rhs);
    DenseMatrixIterator& operator=(const DenseMatrixIterator<T, false>& rhs);

protected:
    void increment();
    void decrement();

    void incrementNonZero();
    void decrementNonZero();

    void advance(const difference_type diff);

    bool equal(const DenseMatrixIterator& rhs) const;
    difference_type distance_to(const DenseMatrixIterator& rhs) const;

    return_type dereference() const;

    std::tuple<size_type, size_type> rowColumn() const;

private:
    DenseMatrix<value_type>* m_matrix;
    size_type m_matrixPos;
};

template <class T, bool Const>
struct matrix_iterator_traits<DenseMatrixIterator<T, Const>>
{
    typedef typename DenseMatrix<T>::value_type value_type;
    typedef typename DenseMatrix<T>::size_type size_type;
    typedef typename DenseMatrix<T>::difference_type difference_type;
    typedef typename std::conditional<Const, typename DenseMatrix<T>::const_reference, typename DenseMatrix<T>::reference>::type return_type;
};

template <class T, bool Const>
DenseMatrixIterator<T, Const>::DenseMatrixIterator(DenseMatrix<T>* matrix, const size_type row, const size_type column):
    MatrixIterator<DenseMatrixIterator<T, Const>>(),
    m_matrix(matrix),
    m_matrixPos(row * m_matrix->columns() + column)
{}

template <class T, bool Const>
DenseMatrixIterator<T, Const>::DenseMatrixIterator(const DenseMatrixIterator<T, false>& rhs):
    MatrixIterator<DenseMatrixIterator<T, Const>>(),
    m_matrix(rhs.m_matrix),
    m_matrixPos(rhs.m_matrixPos)
{}

template <class T, bool Const>
DenseMatrixIterator<T, Const>& DenseMatrixIterator<T, Const>::operator=(const DenseMatrixIterator<T, false>& rhs)
{
    assert(m_matrix == rhs.m_matrix);
    m_matrixPos = rhs.m_matrixPos;
    return *this;
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::increment()
{
    ++m_matrixPos;
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::decrement()
{
    --m_matrixPos;
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::incrementNonZero()
{
    do
    {
        ++m_matrixPos;
    }
    while(m_matrixPos < m_matrix->m_values.size() && !m_matrix->m_values[m_matrixPos]);
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::decrementNonZero()
{
    do
    {
        --m_matrixPos;
    }
    while(m_matrixPos && !m_matrix->m_values[m_matrixPos]);
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::advance(const difference_type diff)
{
    m_matrixPos += diff;
}

template <class T, bool Const>
bool DenseMatrixIterator<T, Const>::equal(const DenseMatrixIterator<T, Const>& rhs) const
{
    return m_matrixPos == rhs.m_matrixPos && m_matrix == rhs.m_matrix;
}

template <class T, bool Const>
auto DenseMatrixIterator<T, Const>::distance_to(const DenseMatrixIterator& rhs) const -> difference_type
{
    return rhs.m_matrixPos - m_matrixPos;
}

template <class T, bool Const>
auto DenseMatrixIterator<T, Const>::dereference() const -> return_type
{
    return m_matrix->m_values[m_matrixPos];
}

template <class T, bool Const>
auto DenseMatrixIterator<T, Const>::rowColumn() const -> std::tuple<size_type, size_type>
{
    ldiv_t d = std::div(static_cast<long>(m_matrixPos), static_cast<long>(m_matrix->columns()));
    return std::make_tuple(static_cast<size_type>(d.quot), static_cast<size_type>(d.rem));
}

#endif
