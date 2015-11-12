#ifndef DENSEMATRIX_H
#define DENSEMATRIX_H

#include "matrix.h"
#include "densematrixtraits.h"

template <class T = double>
class DenseMatrix : public Matrix<DenseMatrix<T>>
{
    friend class Matrix<DenseMatrix<T>>;
    template <class> friend class DenseMatrix;

public:
    typedef typename matrix_traits<DenseMatrix>::value_type value_type;
    typedef typename matrix_traits<DenseMatrix>::size_type size_type;
    typedef typename matrix_traits<DenseMatrix>::difference_type difference_type;
    typedef typename matrix_traits<DenseMatrix>::reference reference;
    typedef typename matrix_traits<DenseMatrix>::const_reference const_reference;
    typedef typename matrix_traits<DenseMatrix>::pointer pointer;
    typedef typename matrix_traits<DenseMatrix>::const_pointer const_pointer;
    typedef typename matrix_traits<DenseMatrix>::acessor acessor;
    typedef typename matrix_traits<DenseMatrix>::const_acessor const_acessor;
    typedef typename matrix_traits<DenseMatrix>::iterator iterator;
    typedef typename matrix_traits<DenseMatrix>::const_iterator const_iterator;

    friend acessor;
    friend const_acessor;
    friend iterator;
    friend const_iterator;

public:
    DenseMatrix(size_type rows, size_type columns, T value = T());

    DenseMatrix(const DenseMatrix& rhs);
    DenseMatrix& operator=(const DenseMatrix& rhs);

    DenseMatrix(DenseMatrix&& rhs);
    DenseMatrix& operator=(DenseMatrix&& rhs);

    template <class D>
    DenseMatrix(const DenseMatrix<D>& rhs);
    template <class D>
    DenseMatrix& operator=(const DenseMatrix<D>& rhs);

    template <class D>
    DenseMatrix(const Matrix<D>& rhs);
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
{}

template <class T>
DenseMatrix<T>::DenseMatrix(const DenseMatrix& rhs):
    Matrix<DenseMatrix>(rhs),
    m_values(rhs.m_values)
{}

template <class T>
DenseMatrix<T>& DenseMatrix<T>::operator=(const DenseMatrix& rhs)
{
    Matrix<DenseMatrix>::operator=(rhs);
    m_values = rhs.m_values;
    return *this;
}

template <class T>
DenseMatrix<T>::DenseMatrix(DenseMatrix&& rhs):
    Matrix<DenseMatrix>(std::move(rhs)),
    m_values(std::move(rhs.m_values))
{}

template <class T>
DenseMatrix<T>& DenseMatrix<T>::operator=(DenseMatrix&& rhs)
{
    Matrix<DenseMatrix>::operator=(std::move(rhs));
    m_values = std::move(rhs.m_values);
    return *this;
}

template <class T>
template <class D>
DenseMatrix<T>::DenseMatrix(const DenseMatrix<D>& rhs):
    Matrix<DenseMatrix>(rhs),
    m_values(rhs.m_values.size())
{
    m_values.assign(rhs.m_values.begin(), rhs.m_values.end());
}

template <class T>
template <class D>
DenseMatrix<T>& DenseMatrix<T>::operator=(const DenseMatrix<D>& rhs)
{
    Matrix<DenseMatrix>::operator=(rhs);
    m_values.assign(rhs.m_values.begin(), rhs.m_values.end());
    return *this;
}

template <class T>
template <class D>
DenseMatrix<T>::DenseMatrix(const Matrix<D>& rhs):
    Matrix<DenseMatrix>(rhs),
    m_values(this->rows() * this->columns())
{
    typename Matrix<D>::const_iterator beg = rhs.begin();
    typename Matrix<D>::const_iterator end = rhs.end();
    while (beg != end)
    {
        *(this)[beg.row()][beg.column()] = *beg;
        ++beg;
    }
}

template <class T>
template <class D>
DenseMatrix<T>& DenseMatrix<T>::operator=(const Matrix<D>& rhs)
{
    Matrix<DenseMatrix>::operator=(rhs);
    this->clear();
    typename Matrix<D>::const_iterator beg = rhs.begin();
    typename Matrix<D>::const_iterator end = rhs.end();
    while (beg != end)
    {
        *(this)[beg.row()][beg.column()] = *beg;
        ++beg;
    }
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
