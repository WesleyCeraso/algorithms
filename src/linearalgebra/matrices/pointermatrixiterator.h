#ifndef POINTERMATRIXITERATOR_H
#define POINTERMATRIXITERATOR_H

#include <vector>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <iomanip>
#include "matrixiterator.h"
#include "pointermatrixelementproxy.h"

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
    PointerMatrixIterator(PointerMatrix<T>* matrix, const size_type row, const size_type column);

    PointerMatrixIterator(const PointerMatrixIterator<T, false>& rhs);
    PointerMatrixIterator& operator=(const PointerMatrixIterator<T, false>& rhs);

protected:
    void increment();
    void decrement();

    void incrementNonZero();
    void decrementNonZero();

    void advance(const difference_type diff);

    bool equal(const PointerMatrixIterator& rhs) const;
    difference_type distance_to(const PointerMatrixIterator& rhs) const;

    return_type dereference() const;

    std::tuple<size_type, size_type> rowColumn() const;

private:
    PointerMatrix<value_type>* m_matrix;
    size_type m_matrixPos;
};

template <class T, bool Const>
struct matrix_iterator_traits<PointerMatrixIterator<T, Const>>
{
    typedef typename PointerMatrix<T>::value_type value_type;
    typedef typename PointerMatrix<T>::size_type size_type;
    typedef typename PointerMatrix<T>::difference_type difference_type;
    typedef typename std::conditional<Const, const PointerMatrixItemProxy<T>, PointerMatrixItemProxy<T>>::type return_type;
};

template <class T, bool Const>
PointerMatrixIterator<T, Const>::PointerMatrixIterator(PointerMatrix<T>* matrix, const size_type row, const size_type column):
    MatrixIterator<PointerMatrixIterator<T, Const>>(),
    m_matrix(matrix),
    m_matrixPos(row * m_matrix->columns() + column)
{}

template <class T, bool Const>
PointerMatrixIterator<T, Const>::PointerMatrixIterator(const PointerMatrixIterator<T, false>& rhs):
    MatrixIterator<PointerMatrixIterator<T, Const>>(),
    m_matrix(rhs.m_matrix),
    m_matrixPos(rhs.m_matrixPos)
{}

template <class T, bool Const>
PointerMatrixIterator<T, Const>& PointerMatrixIterator<T, Const>::operator=(const PointerMatrixIterator<T, false>& rhs)
{
    assert(m_matrix == rhs.m_matrix);
    m_matrixPos = rhs.m_matrixPos;
    return *this;
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::increment()
{
    ++m_matrixPos;
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::decrement()
{
    --m_matrixPos;
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::incrementNonZero()
{
    do
    {
        ++m_matrixPos;
    }
    while(m_matrixPos < m_matrix->m_values.size() && !m_matrix->m_values[m_matrixPos]);
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::decrementNonZero()
{
    do
    {
        --m_matrixPos;
    }
    while(m_matrixPos && !m_matrix->m_values[m_matrixPos]);
}

template <class T, bool Const>
void PointerMatrixIterator<T, Const>::advance(const difference_type diff)
{
    m_matrixPos += diff;
}

template <class T, bool Const>
bool PointerMatrixIterator<T, Const>::equal(const PointerMatrixIterator& rhs) const
{
    return m_matrixPos == rhs.m_matrixPos && m_matrix == rhs.m_matrix;
}

template <class T, bool Const>
auto PointerMatrixIterator<T, Const>::distance_to(const PointerMatrixIterator& rhs) const -> difference_type
{
    return rhs.m_matrixPos - m_matrixPos;
}

template <class T, bool Const>
auto PointerMatrixIterator<T, Const>::dereference() const -> return_type
{
    return return_type(&m_matrix->m_values[m_matrixPos]);
}

template <class T, bool Const>
auto PointerMatrixIterator<T, Const>::rowColumn() const -> std::tuple<size_type, size_type>
{
    ldiv_t d = std::div(static_cast<long>(m_matrixPos), static_cast<long>(m_matrix->columns()));
    return std::make_tuple(static_cast<size_type>(d.quot), static_cast<size_type>(d.rem));
}

#endif
