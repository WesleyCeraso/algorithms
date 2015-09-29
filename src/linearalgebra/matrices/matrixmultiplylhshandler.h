#ifndef MATRIXMULTIPLYLHSHANDLER_H
#define MATRIXMULTIPLYLHSHANDLER_H

template <class T>
class MatrixMultiplyLhsHandler
{
    typedef typename T::const_iterator const_iterator;

public:
    MatrixMultiplyLhsHandler(const T& lhs):
        m_lhs(lhs),
        m_row(typename T::size_type()),
        m_column(typename T::size_type())
    {}

    typename T::size_type row() const
    {
        return m_row;
    }

    typename T::size_type column() const
    {
        return m_column;
    }

    typename T::value_type value() const
    {
        return m_lhs[m_row][m_column];
    }

    void nextRowFirstColumn()
    {
        ++m_row;
    }

    void nextColumnSameRow()
    {
        ++m_column;
    }

    void resetColumn()
    {
        m_column = typename T::size_type();
    }

    bool endRow() const
    {
        return m_column >= m_lhs.columns();
    }

private:
    const T& m_lhs;
    typename T::size_type m_row;
    typename T::size_type m_column;
};

#endif
