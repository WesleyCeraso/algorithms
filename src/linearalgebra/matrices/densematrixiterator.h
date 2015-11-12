#ifndef DENSEMATRIXITERATOR_H
#define DENSEMATRIXITERATOR_H

#include <vector>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include "matrixiterator.h"
#include "densematrixiteratortraits.h"

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
    DenseMatrixIterator(DenseMatrix<T>& matrix, const size_type row);

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    DenseMatrixIterator(const DenseMatrixIterator<T, C>& rhs);
    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    DenseMatrixIterator& operator=(const DenseMatrixIterator<T, C>& rhs);

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    DenseMatrixIterator(DenseMatrixIterator<T, C>&& rhs);
    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    DenseMatrixIterator& operator=(DenseMatrixIterator<T, C>&& rhs);

protected:
    void increment();
    void decrement();

    void advance(const difference_type diff);

    bool equal(const DenseMatrixIterator& rhs) const;
    difference_type distance_to(const DenseMatrixIterator& rhs) const;

    return_type dereference() const;

    size_type currentRow() const;
    size_type currentColumn() const;

private:
    DenseMatrix<value_type>& m_matrix;
    size_type m_index;
};

template <class T, bool Const>
DenseMatrixIterator<T, Const>::DenseMatrixIterator(DenseMatrix<T>& matrix, const size_type row):
    MatrixIterator<DenseMatrixIterator<T, Const>>(),
    m_matrix(matrix),
    m_index(row * m_matrix.columns())
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
DenseMatrixIterator<T, Const>::DenseMatrixIterator(const DenseMatrixIterator<T, C>& rhs):
    MatrixIterator<DenseMatrixIterator<T, Const>>(),
    m_matrix(rhs.m_matrix),
    m_index(rhs.m_index)
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
DenseMatrixIterator<T, Const>& DenseMatrixIterator<T, Const>::operator=(const DenseMatrixIterator<T, C>& rhs)
{
    m_index = rhs.m_index;
    return *this;
}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
DenseMatrixIterator<T, Const>::DenseMatrixIterator(DenseMatrixIterator<T, C>&& rhs):
    MatrixIterator<DenseMatrixIterator<T, Const>>(),
    m_matrix(rhs.m_matrix),
    m_index(std::move(rhs.m_index))
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
DenseMatrixIterator<T, Const>& DenseMatrixIterator<T, Const>::operator=(DenseMatrixIterator<T, C>&& rhs)
{
    m_index = std::move(rhs.m_index);
    return *this;
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::increment()
{
    ++m_index;
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::decrement()
{
    --m_index;
}

template <class T, bool Const>
void DenseMatrixIterator<T, Const>::advance(const difference_type diff)
{
    m_index += diff;
}

template <class T, bool Const>
bool DenseMatrixIterator<T, Const>::equal(const DenseMatrixIterator<T, Const>& rhs) const
{
    return m_index == rhs.m_index;
}

template <class T, bool Const>
auto DenseMatrixIterator<T, Const>::distance_to(const DenseMatrixIterator& rhs) const -> difference_type
{
    return rhs.m_index - m_index;
}

template <class T, bool Const>
auto DenseMatrixIterator<T, Const>::dereference() const -> return_type
{
    return m_matrix.m_values[m_index];
}

template <class T, bool Const>
auto DenseMatrixIterator<T, Const>::currentRow() const -> size_type
{
    return m_index / m_matrix.columns();
}

template <class T, bool Const>
auto DenseMatrixIterator<T, Const>::currentColumn() const -> size_type
{
    return m_index % m_matrix.columns();
}

#endif
