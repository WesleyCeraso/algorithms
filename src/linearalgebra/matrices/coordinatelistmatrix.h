#ifndef COORDINATELISTMATRIX_H
#define COORDINATELISTMATRIX_H

#include "matrix.h"
#include "coordinatelistmatrixiterator.h"

template <class T>
struct matrix_traits<CoordinateListMatrix<T>>
{
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef CoordinateListMatrixIterator<T, false> iterator;
    typedef CoordinateListMatrixIterator<T, true> const_iterator;
};

template <class T = double>
class CoordinateListMatrix : public Matrix<CoordinateListMatrix<T>>
{
    friend class Matrix<CoordinateListMatrix<T>>;
    template<typename> friend class CoordinateListMatrix;
    friend class CoordinateListMatrixIterator<T, false>;
    friend class CoordinateListMatrixIterator<T, true>;

public:
    typedef typename matrix_traits<CoordinateListMatrix>::value_type value_type;
    typedef typename matrix_traits<CoordinateListMatrix>::size_type size_type;
    typedef typename matrix_traits<CoordinateListMatrix>::difference_type difference_type;
    typedef typename matrix_traits<CoordinateListMatrix>::reference reference;
    typedef typename matrix_traits<CoordinateListMatrix>::const_reference const_reference;
    typedef typename matrix_traits<CoordinateListMatrix>::pointer pointer;
    typedef typename matrix_traits<CoordinateListMatrix>::const_pointer const_pointer;

    typedef typename matrix_traits<CoordinateListMatrix>::iterator iterator;
    typedef typename matrix_traits<CoordinateListMatrix>::const_iterator const_iterator;

private:
    typedef std::list<std::tuple<size_type, size_type, T>> container_type;
    typedef typename container_type::iterator container_iterator;
    typedef typename container_type::const_iterator container_const_iterator;

public:
    CoordinateListMatrix(size_type rows, size_type columns, T value = T());

    CoordinateListMatrix(const CoordinateListMatrix& rhs);

    template <class D>
    CoordinateListMatrix(const CoordinateListMatrix<D>& rhs);

    template <class D>
    CoordinateListMatrix(const Matrix<D>& rhs);

    CoordinateListMatrix& operator=(const CoordinateListMatrix& rhs);

    template <class D>
    CoordinateListMatrix& operator=(const CoordinateListMatrix<D>& rhs);

    template <class D>
    CoordinateListMatrix& operator=(const Matrix<D>& rhs);

protected:
    void doClear();
    CoordinateListMatrix doTranspose() const;

private:
    container_type m_values;
};

template <class T>
CoordinateListMatrix<T>::CoordinateListMatrix(size_type rows, size_type columns, T value):
    Matrix<CoordinateListMatrix>(rows, columns)
{
    assert(rows && columns);
    if (value != T())
    {
        for (size_type i = 0; i < this->rows(); ++i)
        {
            for (size_type j = 0; j < this->columns(); ++j)
            {
                m_values.push_back(std::make_tuple(i, j, value));
            }
        }
    }
}

template <class T>
CoordinateListMatrix<T>::CoordinateListMatrix(const CoordinateListMatrix& rhs):
    Matrix<CoordinateListMatrix>(rhs.rows(), rhs.columns()),
    m_values(rhs.m_values)
{}

template <class T>
template <class D>
CoordinateListMatrix<T>::CoordinateListMatrix(const CoordinateListMatrix<D>& rhs):
    Matrix<CoordinateListMatrix>(rhs.rows(), rhs.columns())
{
    m_values.assign(rhs.m_values.begin(), rhs.m_values.end());
}

template <class T>
template <class D>
CoordinateListMatrix<T>::CoordinateListMatrix(const Matrix<D>& rhs):
    Matrix<CoordinateListMatrix>(rhs.rows(), rhs.columns())
{
    typename Matrix<D>::const_iterator beg = rhs.beginNonZero();
    typename Matrix<D>::const_iterator end = rhs.endNonZero();
    for(; beg != end; beg.nextNonZero())
    {
        m_values.push_back(std::make_tuple(beg.row(), beg.column(), *beg));
    }
}

template <class T>
CoordinateListMatrix<T>& CoordinateListMatrix<T>::operator=(const CoordinateListMatrix& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    m_values = rhs.m_values;
    return *this;
}

template <class T>
template <class D>
CoordinateListMatrix<T>& CoordinateListMatrix<T>::operator=(const CoordinateListMatrix<D>& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    m_values.assign(rhs.m_values.begin(), rhs.m_values.end());
    return *this;
}

template <class T>
template <class D>
CoordinateListMatrix<T>& CoordinateListMatrix<T>::operator=(const Matrix<D>& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    this->clear();
    typename Matrix<D>::const_iterator beg = rhs.beginNonZero();
    typename Matrix<D>::const_iterator end = rhs.endNonZero();
    for(; beg != end; beg.nextNonZero())
    {
        m_values.push_back(std::make_tuple(beg.row(), beg.column(), *beg));
    }
    return *this;
}

template <class T>
void CoordinateListMatrix<T>::doClear()
{
    m_values.clear();
}

template <class T>
CoordinateListMatrix<T> CoordinateListMatrix<T>::doTranspose() const
{
    CoordinateListMatrix<T> t(this->columns(), this->rows());

    for (auto v: m_values)
    {
        t.m_values.push_back(std::make_tuple(std::get<1>(v), std::get<0>(v), std::get<2>(v)));
    }
    t.m_values.sort([](const typename container_type::value_type& lhs,
                       const typename container_type::value_type& rhs){
        return std::get<0>(lhs) < std::get<0>(rhs) || (std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) < std::get<1>(rhs));
    });

    return t;
}

#endif
