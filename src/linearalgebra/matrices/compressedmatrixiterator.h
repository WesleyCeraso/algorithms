#ifndef COMPRESSEDMATRIXITERATOR_H
#define COMPRESSEDMATRIXITERATOR_H

#include <vector>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include "matrixiterator.h"

template <class> class CompressedMatrix;

template <class T, bool Const>
class CompressedMatrixIterator : public MatrixIterator<CompressedMatrixIterator<T, Const>>
{
    friend class MatrixIterator<CompressedMatrixIterator>;
    friend class CompressedMatrixIterator<T, true>;
    typedef typename std::conditional<Const, typename CompressedMatrix<T>::container_const_iterator, typename CompressedMatrix<T>::container_iterator>::type iterator_type;

public:
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::value_type value_type;
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::size_type size_type;
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::difference_type difference_type;
    typedef typename matrix_iterator_traits<CompressedMatrixIterator>::return_type return_type;

public:
    CompressedMatrixIterator(CompressedMatrix<T>* matrix, const size_type row, const size_type column);

    CompressedMatrixIterator(const CompressedMatrixIterator<T, false>& rhs);
    CompressedMatrixIterator& operator=(const CompressedMatrixIterator<T, false>& rhs);

protected:
    void increment();
    void decrement();

    void incrementNonZero();
    void decrementNonZero();

    void advance(const difference_type diff);

    bool equal(const CompressedMatrixIterator& rhs) const;
    difference_type distance_to(const CompressedMatrixIterator& rhs) const;

    template <bool C = Const, typename std::enable_if<C, int>::type = 0>
    return_type dereference() const;

    template <bool C = Const, typename std::enable_if<!C, int>::type = 0>
    return_type dereference();

    std::tuple<size_type, size_type> rowColumn() const;

private:
    bool isIteratorSyncronized() const;
    void syncronizeIterator();

private:
    CompressedMatrix<value_type>* m_matrix;
    size_type m_position;
    iterator_type m_iterator;
    static const value_type s_zero;
};

template <class T, bool Const>
struct matrix_iterator_traits<CompressedMatrixIterator<T, Const>>
{
    typedef typename CompressedMatrix<T>::value_type value_type;
    typedef typename CompressedMatrix<T>::size_type size_type;
    typedef typename CompressedMatrix<T>::difference_type difference_type;
    typedef typename std::conditional<Const, typename CompressedMatrix<T>::const_reference, typename CompressedMatrix<T>::reference>::type return_type;
};

template <class T, bool Const>
const typename CompressedMatrixIterator<T, Const>::value_type CompressedMatrixIterator<T, Const>::s_zero = typename CompressedMatrixIterator::value_type();

template <class T, bool Const>
CompressedMatrixIterator<T, Const>::CompressedMatrixIterator(CompressedMatrix<T>* matrix, const size_type row, const size_type column):
    m_matrix(matrix),
    m_position(row * m_matrix->columns() + column),
    m_iterator(m_matrix->m_valueIndex.end())
{
    syncronizeIterator();
}

template <class T, bool Const>
CompressedMatrixIterator<T, Const>::CompressedMatrixIterator(const CompressedMatrixIterator<T, false>& rhs):
    m_matrix(rhs.m_matrix),
    m_position(rhs.m_position),
    m_iterator(rhs.m_iterator)
{}

template <class T, bool Const>
CompressedMatrixIterator<T, Const>& CompressedMatrixIterator<T, Const>::operator=(const CompressedMatrixIterator<T, false>& rhs)
{
    assert(m_matrix == rhs.m_matrix);
    m_position = rhs.m_position;
    m_iterator = rhs.m_iterator;
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::increment()
{
    ++m_position;
    syncronizeIterator();
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::decrement()
{
    --m_position;
    syncronizeIterator();
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::incrementNonZero()
{
    std::tuple<size_type, size_type> rC = rowColumn();
    if (!isIteratorSyncronized())
    {
        iterator_type beg = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC)];
        iterator_type end = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC) + 1];
        m_iterator = std::upper_bound(beg, end, std::get<1>(rC),
                                          [](const size_type& lhs,
                                             const std::pair<value_type, size_type>& rhs){
            return lhs < rhs.second;
        });
    }
    else
    {
        ++m_iterator;
    }

    difference_type diff = std::distance((iterator_type)m_matrix->m_valueIndex.begin(), m_iterator);
    while (std::get<0>(rC) < m_matrix->rows() && static_cast<size_type>(diff) >= m_matrix->m_rowIndex[std::get<0>(rC) + 1]) ++std::get<0>(rC);
    m_position = std::get<0>(rC) * m_matrix->columns() + (m_iterator != m_matrix->m_valueIndex.end() ? m_iterator->second : 0);
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::decrementNonZero()
{
    std::tuple<size_type, size_type> rC = rowColumn();
    if (!isIteratorSyncronized())
    {
        iterator_type beg = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC)];
        iterator_type end = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC) + 1];
        m_iterator = std::lower_bound(beg, end, std::get<1>(rC),
                                      [](const std::pair<value_type, size_type>& lhs,
                                         const size_type& rhs){
            return lhs.second < rhs;
        });
    }

    --m_iterator;

    difference_type diff = std::distance(m_matrix->m_valueIndex.begin(), m_iterator);
    while (diff < m_matrix->m_rowIndex[std::get<0>(rC)]) --std::get<0>(rC);
    m_position = std::get<0>(rC) * m_matrix->m_columns + (m_iterator != m_matrix->m_valueIndex.end() ? m_iterator->second : 0);
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::advance(const difference_type diff)
{
    m_position += diff;
    syncronizeIterator();
}

template <class T, bool Const>
bool CompressedMatrixIterator<T, Const>::equal(const CompressedMatrixIterator<T, Const>& rhs) const
{
    return m_position == rhs.m_position && m_matrix == rhs.m_matrix;
}

template <class T, bool Const>
template <bool C, typename std::enable_if<C, int>::type>
auto CompressedMatrixIterator<T, Const>::dereference() const -> return_type
{
    return isIteratorSyncronized() ? m_iterator->first : s_zero;
}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C, int>::type>
auto CompressedMatrixIterator<T, Const>::dereference() -> return_type
{
    if (!isIteratorSyncronized())
    {
        auto rC = rowColumn();
        iterator_type beg = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC)];
        iterator_type end = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC) + 1];

        m_iterator = std::lower_bound(beg, end, std::get<1>(rC),
                                      [](const std::pair<value_type, size_type>& lhs,
                                         const size_type& rhs){
            return lhs.second < rhs;
        });

        m_iterator = m_matrix->m_valueIndex.insert(m_iterator, std::make_pair(value_type(), std::get<1>(rC)));
        for (size_type i = std::get<0>(rC) + 1; i < m_matrix->m_rowIndex.size(); ++i)
        {
            ++m_matrix->m_rowIndex[i];
        }
    }

    return m_iterator->first;
}

template <class T, bool Const>
auto CompressedMatrixIterator<T, Const>::distance_to(const CompressedMatrixIterator& rhs) const -> difference_type
{
    return rhs.m_position - m_position;
}

template <class T, bool Const>
auto CompressedMatrixIterator<T, Const>::rowColumn() const -> std::tuple<size_type, size_type>
{
    ldiv_t d = std::div(static_cast<long>(m_position), static_cast<long>(m_matrix->columns()));
    return std::make_tuple(static_cast<size_type>(d.quot), static_cast<size_type>(d.rem));
}

template <class T, bool Const>
bool CompressedMatrixIterator<T, Const>::isIteratorSyncronized() const
{
    return m_iterator != m_matrix->m_valueIndex.end();
}

template <class T, bool Const>
void CompressedMatrixIterator<T, Const>::syncronizeIterator()
{
    auto rC = rowColumn();
    if (std::get<0>(rC) >= m_matrix->rows())
    {
        m_iterator = m_matrix->m_valueIndex.end();
        return;
    }

    iterator_type beg = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC)];
    iterator_type end = m_matrix->m_valueIndex.begin() + m_matrix->m_rowIndex[std::get<0>(rC) + 1];

    m_iterator = std::lower_bound(beg, end, std::get<1>(rC),
                                  [](const std::pair<value_type, size_type>& lhs,
                                     const size_type& rhs){
        return lhs.second < rhs;
    });
    if (m_iterator == end || m_iterator->second != std::get<1>(rC)) m_iterator = m_matrix->m_valueIndex.end();
}

#endif
