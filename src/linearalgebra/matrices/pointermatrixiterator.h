#ifndef POINTERMATRIXITERATOR_H
#define POINTERMATRIXITERATOR_H

#include <vector>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include "matrixiterator.h"
#include "pointermatrixiteratortraits.h"

template <class> class PointerMatrix;

template <class T, bool Const>
class PointerMatrixIterator : public MatrixIterator<PointerMatrixIterator<T, Const>>
{
    friend class MatrixIterator<PointerMatrixIterator>;
    friend class PointerMatrixIterator<T, true>;

public:
    typedef typename matrix_iterator_traits<PointerMatrixIterator>::value_type value_type;
    typedef typename matrix_iterator_traits<PointerMatrixIterator>::size_type size_type;
    typedef typename matrix_iterator_traits<PointerMatrixIterator>::difference_type difference_type;
    typedef typename matrix_iterator_traits<PointerMatrixIterator>::return_type return_type;

public:
    PointerMatrixIterator(PointerMatrix<T>& matrix, const size_type row);

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    PointerMatrixIterator(const PointerMatrixIterator<T, C>& rhs);
    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    PointerMatrixIterator& operator=(const PointerMatrixIterator<T, C>& rhs);

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    PointerMatrixIterator(PointerMatrixIterator<T, C>&& rhs);
    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    PointerMatrixIterator& operator=(PointerMatrixIterator<T, C>&& rhs);

protected:
    void increment();
    void decrement();

    void advance(const difference_type diff);

    bool equal(const PointerMatrixIterator& rhs) const;
    difference_type distance_to(const PointerMatrixIterator& rhs) const;

    return_type dereference() const;

    size_type currentRow() const;
    size_type currentColumn() const;

private:
    PointerMatrix<value_type>& m_matrix;
    size_type m_index;
};

template <class T, bool Const>
PointerMatrixIterator<T, Const>::PointerMatrixIterator(PointerMatrix<T>& matrix, const size_type row):
    MatrixIterator<PointerMatrixIterator<T, Const>>(),
    m_matrix(matrix),
    m_index(row * m_matrix.columns())
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
PointerMatrixIterator<T, Const>::PointerMatrixIterator(const PointerMatrixIterator<T, C>& rhs):
    MatrixIterator<PointerMatrixIterator>(rhs),
    m_matrix(rhs.m_matrix),
    m_index(rhs.m_index)
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
PointerMatrixIterator<T, Const>& PointerMatrixIterator<T, Const>::operator=(const PointerMatrixIterator<T, C>& rhs)
{
    MatrixIterator<PointerMatrixIterator>::operator=(rhs);
    m_index = rhs.m_index;
    return *this;
}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
PointerMatrixIterator<T, Const>::PointerMatrixIterator(PointerMatrixIterator<T, C>&& rhs):
    MatrixIterator<PointerMatrixIterator>(std::move(rhs)),
    m_matrix(rhs.m_matrix),
    m_index(std::move(rhs.m_index))
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
PointerMatrixIterator<T, Const>& PointerMatrixIterator<T, Const>::operator=(PointerMatrixIterator<T, C>&& rhs)
{
    MatrixIterator<PointerMatrixIterator>::operator=(std::move(rhs));
    m_index = std::move(rhs.m_index);
    return *this;
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::increment()
{
    ++m_index;
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::decrement()
{
    --m_index;
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::advance(const difference_type diff)
{
    m_index += diff;
}

template <class T, bool Const>
bool PointerMatrixIterator<T, Const>::equal(const PointerMatrixIterator& rhs) const
{
    return m_index == rhs.m_index;
}

template <class T, bool Const>
auto PointerMatrixIterator<T, Const>::distance_to(const PointerMatrixIterator& rhs) const -> difference_type
{
    return rhs.m_index - m_index;
}

template <class T, bool Const>
auto PointerMatrixIterator<T, Const>::dereference() const -> return_type
{
    return return_type(&m_matrix.m_values[m_index]);
}

template <class T, bool Const>
auto PointerMatrixIterator<T, Const>::currentRow() const -> size_type
{
    return m_index / m_matrix.columns();
}

template <class T, bool Const>
auto PointerMatrixIterator<T, Const>::currentColumn() const -> size_type
{
    return m_index % m_matrix.columns();
}

#endif
