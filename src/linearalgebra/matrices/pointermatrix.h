#ifndef POINTERMATRIX_H
#define POINTERMATRIX_H

#include "matrix.h"
#include "pointermatrixtraits.h"

template <class T = double>
class PointerMatrix : public Matrix<PointerMatrix<T>>
{
    friend class Matrix<PointerMatrix<T>>;
    template<typename> friend class PointerMatrix;

public:
    typedef typename matrix_traits<PointerMatrix>::value_type value_type;
    typedef typename matrix_traits<PointerMatrix>::size_type size_type;
    typedef typename matrix_traits<PointerMatrix>::difference_type difference_type;
    typedef typename matrix_traits<PointerMatrix>::reference reference;
    typedef typename matrix_traits<PointerMatrix>::const_reference const_reference;
    typedef typename matrix_traits<PointerMatrix>::pointer pointer;
    typedef typename matrix_traits<PointerMatrix>::const_pointer const_pointer;
    typedef typename matrix_traits<PointerMatrix>::acessor acessor;
    typedef typename matrix_traits<PointerMatrix>::const_acessor const_acessor;
    typedef typename matrix_traits<PointerMatrix>::iterator iterator;
    typedef typename matrix_traits<PointerMatrix>::const_iterator const_iterator;

    friend acessor;
    friend const_acessor;
    friend iterator;
    friend const_iterator;

public:
    PointerMatrix(size_type rows, size_type columns);

    PointerMatrix(const PointerMatrix& rhs);
    PointerMatrix& operator=(const PointerMatrix& rhs);

    PointerMatrix(PointerMatrix&& rhs);
    PointerMatrix& operator=(PointerMatrix&& rhs);

    template <class D>
    PointerMatrix& operator=(const Matrix<D>& rhs);

protected:
    void doClear();
    PointerMatrix doTranspose() const;

private:
    std::vector<T*> m_values;
};

template <class T>
PointerMatrix<T>::PointerMatrix(size_type rows, size_type columns):
    Matrix<PointerMatrix>(rows, columns),
    m_values(this->rows() * this->columns(), nullptr)
{}

template <class T>
PointerMatrix<T>::PointerMatrix(const PointerMatrix& rhs):
    Matrix<PointerMatrix>(rhs),
    m_values(rhs.m_values)
{}

template <class T>
PointerMatrix<T>& PointerMatrix<T>::operator=(const PointerMatrix& rhs)
{
    Matrix<PointerMatrix>::operator=(rhs);
    m_values = rhs.m_values;
    return *this;
}

template <class T>
PointerMatrix<T>::PointerMatrix(PointerMatrix&& rhs):
    Matrix<PointerMatrix>(std::move(rhs)),
    m_values(std::move(rhs.m_values))
{}

template <class T>
PointerMatrix<T>& PointerMatrix<T>::operator=(PointerMatrix&& rhs)
{
    Matrix<PointerMatrix>::operator=(std::move(rhs));
    m_values = std::move(rhs.m_values);
    return *this;
}

template <class T>
template <class D>
PointerMatrix<T>& PointerMatrix<T>::operator=(const Matrix<D>& rhs)
{
    Matrix<PointerMatrix>::operator=(rhs);
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
