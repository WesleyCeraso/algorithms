#ifndef COMPRESSEDMATRIXMULTIPLYRHSHANDLER_H
#define COMPRESSEDMATRIXMULTIPLYRHSHANDLER_H

#include "matrixmultiplyrhshandler.h"

template <class>
class CompressedMatrix;

#ifndef _MSC_VER
template <>
#endif
template<class T>
class MatrixMultiplyRhsHandler<CompressedMatrix<T>>
{
    typedef typename CompressedMatrix<T>::const_iterator const_iterator;

public:
    MatrixMultiplyRhsHandler(const CompressedMatrix<T>& rhs):
        m_rhs(rhs.transpose()),
        m_it(m_rhs.beginNonZero()),
        m_row(typename CompressedMatrix<T>::size_type()),
        m_column(typename CompressedMatrix<T>::size_type())
    {}

    typename CompressedMatrix<T>::size_type row() const
    {
        return m_row;
    }

    typename CompressedMatrix<T>::size_type column() const
    {
        return m_column;
    }

    typename CompressedMatrix<T>::value_type value() const
    {
        return *m_it;
    }

    void nextRowSameColumn()
    {
        m_row = (m_it.nextNonZero()).column();
    }

    void nextColumnFirstRow()
    {
        m_it = m_rhs[++m_column];
        if (m_it != m_rhs.end() && !*m_it) m_it.nextNonZero();
        m_row = m_it.column();
    }

    void resetAll()
    {
        m_it = m_rhs.beginNonZero();
        m_row = m_it.column();
        m_column = m_it.row();
    }

    bool endColumn() const
    {
        return m_column != m_it.row();
    }

private:
    const CompressedMatrix<T> m_rhs;
    const_iterator m_it;
    typename CompressedMatrix<T>::size_type m_row;
    typename CompressedMatrix<T>::size_type m_column;
};

#endif
