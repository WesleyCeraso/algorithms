#ifndef MATRIXMULTIPLYRHSHANDLER_H
#define MATRIXMULTIPLYRHSHANDLER_H

template <class T>
class MatrixMultiplyRhsHandler
{
    typedef typename T::const_iterator const_iterator;

public:
    MatrixMultiplyRhsHandler(const T& rhs):
        m_rhs(rhs),
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
        return m_rhs[m_row][m_column];
    }

    void nextRowSameColumn()
    {
        ++m_row;
    }

    void nextColumnFirstRow()
    {
        m_row = typename T::size_type();
        ++m_column;
    }

    void resetAll()
    {
        m_row = typename T::size_type();
        m_column = typename T::size_type();
    }

    bool endColumn() const
    {
        return m_row >= m_rhs.rows();
    }

private:
    const T& m_rhs;
    typename T::size_type m_row;
    typename T::size_type m_column;
};

#endif
