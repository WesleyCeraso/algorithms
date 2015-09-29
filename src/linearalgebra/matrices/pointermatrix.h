#ifndef POINTERMATRIX_H
#define POINTERMATRIX_H

#include "pointermatrixiterator.h"
#include "matrix.h"

template <class T>
struct matrix_traits<PointerMatrix<T>>
{
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef PointerMatrixIterator<T, false> iterator;
    typedef PointerMatrixIterator<T, true> const_iterator;
};

template <class T = double>
class PointerMatrix : public Matrix<PointerMatrix<T>>
{
    friend class Matrix<PointerMatrix<T>>;
    template<typename> friend class PointerMatrix;
    friend class PointerMatrixIterator<T, false>;
    friend class PointerMatrixIterator<T, true>;

public:
    typedef typename matrix_traits<PointerMatrix>::value_type value_type;
    typedef typename matrix_traits<PointerMatrix>::size_type size_type;
    typedef typename matrix_traits<PointerMatrix>::difference_type difference_type;
    typedef typename matrix_traits<PointerMatrix>::reference reference;
    typedef typename matrix_traits<PointerMatrix>::const_reference const_reference;
    typedef typename matrix_traits<PointerMatrix>::pointer pointer;
    typedef typename matrix_traits<PointerMatrix>::const_pointer const_pointer;

    typedef typename matrix_traits<PointerMatrix>::iterator iterator;
    typedef typename matrix_traits<PointerMatrix>::const_iterator const_iterator;

public:
    PointerMatrix(size_type rows, size_type columns);

    PointerMatrix(const PointerMatrix& rhs);

    template <class D>
    PointerMatrix(Matrix<D>* rhs);

    PointerMatrix& operator=(const PointerMatrix& rhs);

    template <class D>
    PointerMatrix& operator=(const Matrix<D>& rhs);

    template <class D>
    PointerMatrix& operator=(Matrix<D>* rhs);

protected:
    void doClear();
    PointerMatrix doTranspose() const;

private:
    std::vector<T*> m_values;
};

template <class T>
PointerMatrix<T>::PointerMatrix(size_type rows, size_type columns):
    Matrix<PointerMatrix<T>>(rows, columns),
    m_values(this->rows() * this->columns(), nullptr)
{}

template <class T>
PointerMatrix<T>::PointerMatrix(const PointerMatrix& rhs):
    Matrix<PointerMatrix<T>>(rhs.rows(), rhs.columns()),
    m_values(rhs.m_values)
{}

template <class T>
template <class D>
PointerMatrix<T>::PointerMatrix(Matrix<D>* rhs):
    Matrix<PointerMatrix<T>>(rhs->rows(), rhs->columns()),
    m_values(this->rows() * this->columns, nullptr)
{
    iterator lBeg = this->begin();
    iterator lEnd = this->end();
    typename Matrix<D>::iterator rBeg = rhs->begin();
    while (lBeg != lEnd)
    {
        *lBeg = &(*rBeg);
        ++lBeg;
        ++rBeg;
    }
}

template <class T>
PointerMatrix<T>& PointerMatrix<T>::operator=(const PointerMatrix& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    m_values = rhs.m_values;
    return *this;
}

template <class T>
template <class D>
PointerMatrix<T>& PointerMatrix<T>::operator=(const Matrix<D>& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    this->clear();
    iterator lBeg = this->begin();
    iterator lEnd = this->end();
    typename Matrix<D>::const_iterator rBeg = rhs.begin();
    while (lBeg != lEnd)
    {
        *lBeg = *rBeg;
        ++lBeg;
        ++rBeg;
    }

    return *this;
}

template <class T>
template <class D>
PointerMatrix<T>& PointerMatrix<T>::operator=(Matrix<D>* rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    iterator lBeg = this->begin();
    iterator lEnd = this->end();
    typename Matrix<D>::iterator rBeg = rhs->begin();
    while (lBeg != lEnd)
    {
        *lBeg = &(*rBeg);
        ++lBeg;
        ++rBeg;
    }

    return *this;
}

template <class T>
PointerMatrix<T> PointerMatrix<T>::doTranspose() const
{
    PointerMatrix<T> t(this->columns(), this->rows());

    for(size_type i = 0; i < this->rows(); ++i)
    {
        for(size_type j = 0; j < this->columns(); ++j)
        {
            t.m_values[(j * t.rows()) + i] = m_values[(i * this->columns()) + j];
        }
    }

    return t;
}

template <class T>
void PointerMatrix<T>::doClear()
{
    for(T* p: m_values)
    {
        *p = T();
    }
}

#endif
