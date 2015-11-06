#ifndef COMPRESSEDMATRIXMULTIPLYLHSHANDLER_H
#define COMPRESSEDMATRIXMULTIPLYLHSHANDLER_H

#include "matrixmultiplylhshandler.h"

template <class>
class CompressedMatrix;

#ifndef _MSC_VER
template <>
#endif
template<class T>
class MatrixMultiplyLhsHandler<CompressedMatrix<T>>
{
    typedef typename CompressedMatrix<T>::const_iterator const_iterator;

public:
    MatrixMultiplyLhsHandler(const CompressedMatrix<T>& lhs):
        m_lhs(lhs),
        m_it(m_lhs.beginNonZero()),
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

    void nextRowFirstColumn()
    {
        m_it = m_lhs[++m_row];
        if (m_it != m_lhs.end() && !*m_it) m_it.nextNonZero();
        m_column = m_it.column();
    }

    void nextColumnSameRow()
    {
        m_column = (m_it.nextNonZero()).column();
    }

    void resetColumn()
    {
        m_it = m_lhs[m_row];
        if (m_it != m_lhs.end() && !*m_it) m_it.nextNonZero();
        m_column = m_it.column();
    }

    bool endRow() const
    {
        return m_row != m_it.row();
    }

private:
    const CompressedMatrix<T>& m_lhs;
    const_iterator m_it;
    typename CompressedMatrix<T>::size_type m_row;
    typename CompressedMatrix<T>::size_type m_column;
};

#endif
