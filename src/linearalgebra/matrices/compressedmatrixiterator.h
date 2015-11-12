#ifndef COMPRESSEDMATRIXITERATOR_H
#define COMPRESSEDMATRIXITERATOR_H

#include <cassert>
#include "matrixiterator.h"
#include "compressedmatrixiteratortraits.h"

template <class T, bool Const>
class CompressedMatrixIterator : public MatrixIterator<CompressedMatrixIterator<T, Const>>
{
    friend class MatrixIterator<CompressedMatrixIterator>;
    friend class CompressedMatrixIterator<T, true>;

public:
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::value_type value_type;
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::size_type size_type;
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::difference_type difference_type;
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::return_type return_type;

public:
    CompressedMatrixIterator(CompressedMatrix<T>& matrix, size_type row);

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    CompressedMatrixIterator(const CompressedMatrixIterator<T, C>& rhs);
    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    CompressedMatrixIterator& operator=(const CompressedMatrixIterator<T, C>& rhs);

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    CompressedMatrixIterator(CompressedMatrixIterator<T, C>&& rhs);
    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    CompressedMatrixIterator& operator=(CompressedMatrixIterator<T, C>&& rhs);

protected:
    void increment();
    void decrement();

    void advance(const difference_type diff);

    bool equal(const CompressedMatrixIterator& rhs) const;
    difference_type distance_to(const CompressedMatrixIterator& rhs) const;

    return_type dereference() const;

    size_type currentRow() const;
    size_type currentColumn() const;

private:
    void incrementRow();
    void decrementRow();
    size_type rowByIndex() const;

private:
    CompressedMatrix<T>& m_matrix;
    size_type m_index;
    size_type m_row;
};

template <class T, bool Const>
CompressedMatrixIterator<T, Const>::CompressedMatrixIterator(CompressedMatrix<T>& matrix, size_type row):
    m_matrix(matrix),
    m_index(m_matrix.m_rowIndex[row]),
    m_row(rowByIndex())
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
CompressedMatrixIterator<T, Const>::CompressedMatrixIterator(const CompressedMatrixIterator<T, C>& rhs):
    m_matrix(rhs.m_matrix),
    m_index(rhs.m_index),
    m_row(rhs.m_row)
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
CompressedMatrixIterator<T, Const>& CompressedMatrixIterator<T, Const>::operator=(const CompressedMatrixIterator<T, C>& rhs)
{
    m_index = rhs.m_index;
    m_row = rhs.m_row;
    return *this;
}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
CompressedMatrixIterator<T, Const>::CompressedMatrixIterator(CompressedMatrixIterator<T, C>&& rhs):
    m_matrix(rhs.m_matrix),
    m_index(std::move(rhs.m_index)),
    m_row(std::move(rhs.m_row))
{}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C || Const>::type*>
CompressedMatrixIterator<T, Const>& CompressedMatrixIterator<T, Const>::operator=(CompressedMatrixIterator<T, C>&& rhs)
{
    m_index = std::move(rhs.m_index);
    m_row = std::move(rhs.m_row);
    return *this;
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::increment()
{
    ++m_index;

    if (m_index == m_matrix.m_rowIndex[m_row + 1])
        incrementRow();
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::decrement()
{
    if (m_index == m_matrix.m_rowIndex[m_row])
        decrementRow();

    --m_index;
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::advance(const difference_type diff)
{
    difference_type local_diff(diff);
    if (local_diff > 0)
        while (local_diff--) increment();
    else
        while (local_diff++) decrement();
}

template <class T, bool Const>
bool CompressedMatrixIterator<T, Const>::equal(const CompressedMatrixIterator<T, Const>& rhs) const
{
    return m_index == rhs.m_index;
}

template <class T, bool Const>
auto CompressedMatrixIterator<T, Const>::dereference() const -> return_type
{
    return m_matrix.m_valueIndex[m_index].first;
}

template <class T, bool Const>
auto CompressedMatrixIterator<T, Const>::distance_to(const CompressedMatrixIterator& rhs) const -> difference_type
{
    return rhs.m_index - m_index;
}

template <class T, bool Const>
auto CompressedMatrixIterator<T, Const>::currentRow() const -> size_type
{
    return m_row;
}

template <class T, bool Const>
auto CompressedMatrixIterator<T, Const>::currentColumn() const -> size_type
{
    return m_matrix.m_valueIndex[m_index].second;
}

template <class T, bool Const>
inline void CompressedMatrixIterator<T, Const>::incrementRow()
{
    size_type rows = m_matrix.rows();
    while (++m_row < rows)
    {
        if (m_matrix.m_rowIndex[m_row] != m_matrix.m_rowIndex[m_row + 1])
            break;
    }
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::decrementRow()
{
    while(--m_row)
    {
        if (m_matrix.m_rowIndex[m_row] != m_matrix.m_rowIndex[m_row + 1])
            break;
    }
}

template <class T, bool Const>
auto CompressedMatrixIterator<T, Const>::rowByIndex() const -> size_type
{
    return std::distance(m_matrix.m_rowIndex.begin(), std::upper_bound(m_matrix.m_rowIndex.begin(), m_matrix.m_rowIndex.end(), m_index, [](const size_type lhs, const size_type rhs)
    {
        return lhs < rhs;
    })) - 1;
}

#endif
