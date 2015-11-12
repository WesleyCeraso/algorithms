#ifndef COMPRESSEDMATRIXLEFTMULTIPLIERADAPTOR_H
#define COMPRESSEDMATRIXLEFTMULTIPLIERADAPTOR_H

#include "matrixleftmultiplieradaptor.h"

template <class>
class CompressedMatrix;

#ifndef _MSC_VER
template <>
#endif
template<class T>
class MatrixLeftMultiplierAdaptor<CompressedMatrix<T>>
{
    typedef typename CompressedMatrix<T>::const_iterator const_iterator;
    typedef typename CompressedMatrix<T>::size_type size_type;
    typedef typename CompressedMatrix<T>::value_type value_type;

public:
    MatrixLeftMultiplierAdaptor(const CompressedMatrix<T>& lhs):
        m_lhs(lhs),
        m_curr(lhs.begin()),
        m_row(typename CompressedMatrix<T>::size_type())
    {}

    size_type row() const
    {
        return m_row;
    }

    size_type column() const
    {
        return m_curr.column();
    }

    value_type value() const
    {
        return *m_curr;
    }

    void beginRow(size_type row)
    {
        m_row = row;
        m_curr = m_lhs.begin(m_row);
    }

    void nextColumn()
    {
        ++m_curr;
    }

    bool endRow() const
    {
        return m_row != m_curr.row();
    }

private:
    const CompressedMatrix<T>& m_lhs;
    const_iterator m_curr;
    size_type m_row;
};

#endif
