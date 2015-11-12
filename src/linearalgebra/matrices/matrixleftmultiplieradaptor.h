#ifndef MATRIXLEFTMULTIPLIERADAPTOR_H
#define MATRIXLEFTMULTIPLIERADAPTOR_H

template <class T>
class MatrixLeftMultiplierAdaptor
{
    typedef typename T::const_iterator const_iterator;

public:
    MatrixLeftMultiplierAdaptor(const T& lhs):
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

    void beginRow(typename T::size_type row)
    {
        m_row = row;
        m_column = 0;
    }

    void nextColumn()
    {
        ++m_column;
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
