#ifndef COORDINATELISTMATRIXITERATOR_H
#define COORDINATELISTMATRIXITERATOR_H

#include <list>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <iomanip>

template <class> class CoordinateListMatrix;

template <class T, bool Const>
class CoordinateListMatrixIterator : public MatrixIterator<CoordinateListMatrixIterator<T, Const>>
{
    friend class MatrixIterator<CoordinateListMatrixIterator>;
    friend class CoordinateListMatrixIterator<T, true>;
    typedef typename std::conditional<Const, typename CoordinateListMatrix<T>::container_const_iterator, typename CoordinateListMatrix<T>::container_iterator>::type iterator_type;

public:
    typedef typename matrix_iterator_traits<CoordinateListMatrixIterator>::value_type value_type;
    typedef typename matrix_iterator_traits<CoordinateListMatrixIterator>::size_type size_type;
    typedef typename matrix_iterator_traits<CoordinateListMatrixIterator>::difference_type difference_type;
    typedef typename matrix_iterator_traits<CoordinateListMatrixIterator>::return_type return_type;

public:
    CoordinateListMatrixIterator(CoordinateListMatrix<T>* matrix, const size_type row, const size_type column);

    CoordinateListMatrixIterator(const CoordinateListMatrixIterator<T, false>& rhs);
    CoordinateListMatrixIterator& operator=(const CoordinateListMatrixIterator<T, false>& rhs);

protected:
    void increment();
    void decrement();

    void incrementNonZero();
    void decrementNonZero();

    void advance(const difference_type diff);

    bool equal(const CoordinateListMatrixIterator& rhs) const;
    difference_type distance_to(const CoordinateListMatrixIterator& rhs) const;

    template <bool C = Const, typename std::enable_if<C, int>::type = 0>
    return_type dereference() const;

    template <bool C = Const, typename std::enable_if<!C, int>::type = 0>
    return_type dereference();

    std::tuple<size_type, size_type> rowColumn() const;

private:
    bool isIteratorSyncronized() const;
    void syncronizePosition();
    void syncronizeIterator();
    size_type iteratorPosition() const;

private:
    CoordinateListMatrix<value_type>* m_matrix;
    size_type m_position;
    iterator_type m_iterator;
    static const value_type s_zero;
};

template <class T, bool Const>
struct matrix_iterator_traits<CoordinateListMatrixIterator<T, Const>>
{
    typedef typename CoordinateListMatrix<T>::value_type value_type;
    typedef typename CoordinateListMatrix<T>::size_type size_type;
    typedef typename CoordinateListMatrix<T>::difference_type difference_type;
    typedef typename std::conditional<Const, typename CoordinateListMatrix<T>::const_reference, typename CoordinateListMatrix<T>::reference>::type return_type;
};

template <class T, bool Const>
const typename CoordinateListMatrixIterator<T, Const>::value_type CoordinateListMatrixIterator<T, Const>::s_zero = typename CoordinateListMatrixIterator::value_type();

template <class T, bool Const>
CoordinateListMatrixIterator<T, Const>::CoordinateListMatrixIterator(CoordinateListMatrix<T>* matrix, const size_type row, const size_type column):
    m_matrix(matrix),
    m_position(row * m_matrix->columns() + column),
    m_iterator(m_matrix->m_values.end())
{
    syncronizeIterator();
}

template <class T, bool Const>
CoordinateListMatrixIterator<T, Const>::CoordinateListMatrixIterator(const CoordinateListMatrixIterator<T, false>& rhs):
    m_matrix(rhs.m_matrix),
    m_position(rhs.m_position),
    m_iterator(rhs.m_iterator)
{}

template <class T, bool Const>
CoordinateListMatrixIterator<T, Const>& CoordinateListMatrixIterator<T, Const>::operator=(const CoordinateListMatrixIterator<T, false>& rhs)
{
    assert(m_matrix == rhs.m_matrix);
    m_position = rhs.m_position;
    m_iterator = rhs.m_iterator;
    return *this;
}

template <class T, bool Const>
void CoordinateListMatrixIterator<T, Const>::increment()
{
    ++m_position;
    syncronizeIterator();
}

template <class T, bool Const>
void CoordinateListMatrixIterator<T, Const>::decrement()
{
    --m_position;
    syncronizeIterator();
}

template <class T, bool Const>
void CoordinateListMatrixIterator<T, Const>::incrementNonZero()
{
    if (!isIteratorSyncronized())
    {
        m_iterator = std::upper_bound(m_matrix->m_values.begin(), m_matrix->m_values.end(), rowColumn(),
                                          [](const std::tuple<size_type, size_type>& lhs,
                                             const typename iterator_type::value_type& rhs){
            return std::get<0>(lhs) < std::get<0>(rhs) || (std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) < std::get<1>(rhs));
        });
    }
    else
    {
        ++m_iterator;
    }

    syncronizePosition();
}

template <class T, bool Const>
void CoordinateListMatrixIterator<T, Const>::decrementNonZero()
{
    if (!isIteratorSyncronized())
    {
        m_iterator = std::lower_bound(m_matrix->m_values.begin(), m_matrix->m_values.end(), rowColumn(),
                                      [](const typename iterator_type::value_type& lhs,
                                         const std::tuple<size_type, size_type>& rhs){
            return std::get<0>(lhs) < std::get<0>(rhs) || (std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) < std::get<1>(rhs));
        });
    }

    --m_iterator;
    syncronizePosition();
}

template <class T, bool Const>
void CoordinateListMatrixIterator<T, Const>::advance(const difference_type diff)
{
    m_position += diff;
    syncronizeIterator();
}

template <class T, bool Const>
bool CoordinateListMatrixIterator<T, Const>::equal(const CoordinateListMatrixIterator<T, Const>& rhs) const
{
    return m_position == rhs.m_position && m_matrix == rhs.m_matrix;
}

template <class T, bool Const>
template <bool C, typename std::enable_if<C, int>::type>
auto CoordinateListMatrixIterator<T, Const>::dereference() const -> return_type
{
    return isIteratorSyncronized() ? std::get<2>(*m_iterator) : s_zero;
}

template <class T, bool Const>
template <bool C, typename std::enable_if<!C, int>::type>
auto CoordinateListMatrixIterator<T, Const>::dereference() -> return_type
{
    if (!isIteratorSyncronized())
    {
        std::tuple<size_type, size_type> rC = rowColumn();
        m_iterator = std::lower_bound(m_matrix->m_values.begin(), m_matrix->m_values.end(), rC,
                                      [](const typename iterator_type::value_type& lhs,
                                         const std::tuple<size_type, size_type>& rhs){
            return std::get<0>(lhs) < std::get<0>(rhs) || (std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) < std::get<1>(rhs));
        });
        m_iterator = m_matrix->m_values.insert(m_iterator, std::make_tuple(std::get<0>(rC), std::get<1>(rC), value_type()));
    }
    return std::get<2>(*m_iterator);
}

template <class T, bool Const>
auto CoordinateListMatrixIterator<T, Const>::distance_to(const CoordinateListMatrixIterator& rhs) const -> difference_type
{
    return rhs.m_position - m_position;
}

template <class T, bool Const>
auto CoordinateListMatrixIterator<T, Const>::rowColumn() const -> std::tuple<size_type, size_type>
{
    ldiv_t d = std::div(static_cast<long>(m_position), static_cast<long>(m_matrix->columns()));
    return std::make_tuple(static_cast<size_type>(d.quot), static_cast<size_type>(d.rem));
}

template <class T, bool Const>
bool CoordinateListMatrixIterator<T, Const>::isIteratorSyncronized() const
{
    return m_iterator != m_matrix->m_values.end();
}

template <class T, bool Const>
void CoordinateListMatrixIterator<T, Const>::syncronizePosition()
{
    m_position = iteratorPosition();
}

template <class T, bool Const>
void CoordinateListMatrixIterator<T, Const>::syncronizeIterator()
{
    m_iterator = std::lower_bound(m_matrix->m_values.begin(), m_matrix->m_values.end(), rowColumn(),
                                  [](const typename iterator_type::value_type& lhs,
                                     const std::tuple<size_type, size_type>& rhs){
        return std::get<0>(lhs) < std::get<0>(rhs) || (std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) < std::get<1>(rhs));
    });
    if (iteratorPosition() != m_position) m_iterator = m_matrix->m_values.end();
}

template <class T, bool Const>
auto CoordinateListMatrixIterator<T, Const>::iteratorPosition() const -> size_type
{
    return m_iterator != m_matrix->m_values.end() ? std::get<0>(*m_iterator) * m_matrix->columns() + std::get<1>(*m_iterator) : m_matrix->rows() * m_matrix->columns();
}

#endif
