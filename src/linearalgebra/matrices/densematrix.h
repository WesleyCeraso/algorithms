#ifndef DENSEMATRIX_H
#define DENSEMATRIX_H

#include "matrix.h"
#include "densematrixiterator.h"

template <class T>
struct matrix_traits<DenseMatrix<T>>
{
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef DenseMatrixIterator<T, false> iterator;
    typedef DenseMatrixIterator<T, true> const_iterator;
};

template <class T = double>
class DenseMatrix : public Matrix<DenseMatrix<T>>
{
    friend class Matrix<DenseMatrix<T>>;
    template <class> friend class DenseMatrix;
    friend class DenseMatrixIterator<T, false>;
    friend class DenseMatrixIterator<T, true>;

public:
    typedef typename matrix_traits<DenseMatrix>::value_type value_type;
    typedef typename matrix_traits<DenseMatrix>::size_type size_type;
    typedef typename matrix_traits<DenseMatrix>::difference_type difference_type;
    typedef typename matrix_traits<DenseMatrix>::reference reference;
    typedef typename matrix_traits<DenseMatrix>::const_reference const_reference;
    typedef typename matrix_traits<DenseMatrix>::pointer pointer;
    typedef typename matrix_traits<DenseMatrix>::const_pointer const_pointer;

    typedef typename matrix_traits<DenseMatrix>::iterator iterator;
    typedef typename matrix_traits<DenseMatrix>::const_iterator const_iterator;

public:
    DenseMatrix(size_type rows, size_type columns, T value = T());

    DenseMatrix(const DenseMatrix& rhs);

    template <class D>
    DenseMatrix(const DenseMatrix<D>& rhs);

    template <class D>
    DenseMatrix(const Matrix<D>& rhs);

    DenseMatrix& operator=(const DenseMatrix& rhs);

    template <class D>
    DenseMatrix& operator=(const DenseMatrix<D>& rhs);

    template <class D>
    DenseMatrix& operator=(const Matrix<D>& rhs);

protected:
    void doClear();
    DenseMatrix doTranspose() const;

private:
    std::vector<T> m_values;
};

template <class T>
DenseMatrix<T>::DenseMatrix(size_type rows, size_type columns, T value):
    Matrix<DenseMatrix>(rows, columns),
    m_values(rows * columns, value)
{
    assert(rows && columns);
}

template <class T>
DenseMatrix<T>::DenseMatrix(const DenseMatrix& rhs):
    Matrix<DenseMatrix>(rhs.rows(), rhs.columns()),
    m_values(rhs.m_values)
{}

template <class T>
template <class D>
DenseMatrix<T>::DenseMatrix(const DenseMatrix<D>& rhs):
    Matrix<DenseMatrix>(rhs.rows(), rhs.columns()),
    m_values(rhs.m_values.size())
{
    for(size_type i = 0; i < rhs.m_values.size(); ++i)
    {
        m_values[i] = rhs.m_values[i];
    }
}

template <class T>
template <class D>
DenseMatrix<T>::DenseMatrix(const Matrix<D>& rhs):
    Matrix<DenseMatrix>(rhs.rows(), rhs.columns()),
    m_values(this->rows() * this->columns())
{
    typename Matrix<D>::const_iterator beg = rhs.begin();
    typename Matrix<D>::const_iterator end = rhs.end();
    for(; beg != end; beg.nextNonZero())
    {
        *(this)[beg.row()][beg.column()] = *beg;
    }
}

template <class T>
DenseMatrix<T>& DenseMatrix<T>::operator=(const DenseMatrix& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    m_values = rhs.m_values;
    return *this;
}

template <class T>
template <class D>
DenseMatrix<T>& DenseMatrix<T>::operator=(const DenseMatrix<D>& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    for(size_type i = 0; i < rhs.m_values.size(); ++i)
    {
        m_values[i] = rhs.m_values[i];
    }
    return *this;
}

template <class T>
template <class D>
DenseMatrix<T>& DenseMatrix<T>::operator=(const Matrix<D>& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    this->clear();
    typename Matrix<D>::const_iterator beg = rhs.begin();
    typename Matrix<D>::const_iterator end = rhs.end();
    for(; beg != end; beg.nextNonZero())
    {
        *(this)[beg.row()][beg.column()] = *beg;
    }
    return *this;
}

template <class T>
void DenseMatrix<T>::doClear()
{
    std::fill(m_values.begin(), m_values.end(), T());
}

template <class T>
DenseMatrix<T> DenseMatrix<T>::doTranspose() const
{
    DenseMatrix<T> t(this->columns(), this->rows());

    for(size_type i = 0; i < this->rows(); ++i)
    {
        for(size_type j = 0; j < this->columns(); ++j)
        {
            t.m_values[(j * this->rows()) + i] = m_values[(i * this->columns()) + j];
        }
    }

    return t;
}

#endif
