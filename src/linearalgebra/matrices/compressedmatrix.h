#ifndef COMPRESSEDMATRIX_H
#define COMPRESSEDMATRIX_H

#include "matrix.h"
#include "compressedmatrixtraits.h"

template <class T = double>
class CompressedMatrix : public Matrix<CompressedMatrix<T>>
{
    friend class Matrix<CompressedMatrix<T>>;
    template<typename> friend class CompressedMatrix;

public:
    typedef typename matrix_traits<CompressedMatrix>::value_type value_type;
    typedef typename matrix_traits<CompressedMatrix>::size_type size_type;
    typedef typename matrix_traits<CompressedMatrix>::difference_type difference_type;
    typedef typename matrix_traits<CompressedMatrix>::reference reference;
    typedef typename matrix_traits<CompressedMatrix>::const_reference const_reference;
    typedef typename matrix_traits<CompressedMatrix>::pointer pointer;
    typedef typename matrix_traits<CompressedMatrix>::const_pointer const_pointer;
    typedef typename matrix_traits<CompressedMatrix>::acessor acessor;
    typedef typename matrix_traits<CompressedMatrix>::const_acessor const_acessor;
    typedef typename matrix_traits<CompressedMatrix>::iterator iterator;
    typedef typename matrix_traits<CompressedMatrix>::const_iterator const_iterator;

    friend acessor;
    friend const_acessor;
    friend iterator;
    friend const_iterator;

public:
    CompressedMatrix(size_type rows, size_type columns, T value = T());

    CompressedMatrix(size_type rows, size_type columns, const std::vector<std::tuple<value_type, size_type>>& valueColumnIndex, const std::vector<size_type>& rowIndex);
    CompressedMatrix(size_type rows, size_type columns, std::vector<std::tuple<value_type, size_type>>&& valueColumnIndex, std::vector<size_type>&& rowIndex);

    CompressedMatrix(size_type rows, size_type columns, const std::vector<std::tuple<size_type, size_type, value_type>>& triplets);

    CompressedMatrix(const CompressedMatrix& rhs);
    CompressedMatrix& operator=(const CompressedMatrix& rhs);

    CompressedMatrix(CompressedMatrix&& rhs);
    CompressedMatrix& operator=(CompressedMatrix&& rhs);

    template <class D>
    CompressedMatrix(const CompressedMatrix<D>& rhs);
    template <class D>
    CompressedMatrix& operator=(const CompressedMatrix<D>& rhs);

    template <class D>
    CompressedMatrix(const Matrix<D>& rhs);
    template <class D>
    CompressedMatrix& operator=(const Matrix<D>& rhs);

protected:
    void doClear();
    CompressedMatrix doTranspose() const;

private:
    std::vector<std::pair<T, size_type>> m_valueIndex;
    std::vector<size_type> m_rowIndex;
};

template <class T>
CompressedMatrix<T>::CompressedMatrix(size_type rows, size_type columns, T value):
    Matrix<CompressedMatrix>(rows, columns),
    m_valueIndex(value_type(value) != value_type() ? rows * columns : size_type()),
    m_rowIndex(rows + 1)
{
    if (value_type(value) != value_type())
    {
        for (size_type i = size_type(); i < rows; ++i)
        {
            for (size_type j = size_type(); j < columns; ++j)
            {
                m_valueIndex[i * columns + j] = (std::make_pair(value, j));
            }
            m_rowIndex[i + 1] = columns * (i + 1);
        }
    }
}

template <class T>
CompressedMatrix<T>::CompressedMatrix(size_type rows, size_type columns, const std::vector<std::tuple<value_type, size_type>>& valueColumnIndex, const std::vector<size_type>& rowIndex):
    Matrix<CompressedMatrix>(rows, columns),
    m_valueIndex(valueColumnIndex),
    m_rowIndex(rowIndex)
{}

template <class T>
CompressedMatrix<T>::CompressedMatrix(size_type rows, size_type columns, std::vector<std::tuple<value_type, size_type>>&& valueColumnIndex, std::vector<size_type>&& rowIndex):
    Matrix<CompressedMatrix>(rows, columns),
    m_valueIndex(std::move(valueColumnIndex)),
    m_rowIndex(std::move(rowIndex))
{}

template <class T>
CompressedMatrix<T>::CompressedMatrix(size_type rows, size_type columns, const std::vector<std::tuple<size_type, size_type, value_type>>& triplets):
    Matrix<CompressedMatrix>(rows, columns),
    m_valueIndex(triplets.size()),
    m_rowIndex(rows + 1)
{
    for (size_type i = size_type(); i < triplets.size(); ++i)
    {
        m_valueIndex[i] = std::make_pair(std::get<2>(triplets[i]), std::get<1>(triplets[i]));
        ++m_rowIndex[std::get<0>(triplets[i]) + 1];
    }

    for (size_type i = 1; i < m_rowIndex.size(); ++i)
    {
        m_rowIndex[i] += m_rowIndex[i - 1];
    }
}

template <class T>
CompressedMatrix<T>::CompressedMatrix(const CompressedMatrix& rhs):
    Matrix<CompressedMatrix>(rhs),
    m_valueIndex(rhs.m_valueIndex),
    m_rowIndex(rhs.m_rowIndex)
{}

template <class T>
CompressedMatrix<T>& CompressedMatrix<T>::operator=(const CompressedMatrix& rhs)
{
    Matrix<CompressedMatrix>::operator=(rhs);
    m_rowIndex = rhs.m_rowIndex;
    m_valueIndex = rhs.m_valueIndex;
    return *this;
}

template <class T>
CompressedMatrix<T>::CompressedMatrix(CompressedMatrix&& rhs):
    Matrix<CompressedMatrix>(std::move(rhs)),
    m_valueIndex(std::move(rhs.m_valueIndex)),
    m_rowIndex(std::move(rhs.m_rowIndex))
{}

template <class T>
CompressedMatrix<T>& CompressedMatrix<T>::operator=(CompressedMatrix&& rhs)
{
    Matrix<CompressedMatrix>::operator=(std::move(rhs));
    m_rowIndex = std::move(rhs.m_rowIndex);
    m_valueIndex = std::move(rhs.m_valueIndex);
    return *this;
}

template <class T>
template <class D>
CompressedMatrix<T>::CompressedMatrix(const CompressedMatrix<D>& rhs):
    Matrix<CompressedMatrix>(rhs.rows(), rhs.columns()),
    m_valueIndex(rhs.m_valueIndex.size()),
    m_rowIndex(rhs.m_rowIndex)
{
    m_valueIndex.assign(rhs.m_valueIndex.begin(), rhs.m_valueIndex.end());
}

template <class T>
template <class D>
CompressedMatrix<T>& CompressedMatrix<T>::operator=(const CompressedMatrix<D>& rhs)
{
    Matrix<CompressedMatrix>::operator=(rhs);
    m_rowIndex = rhs.m_rowIndex;
    m_valueIndex.assign(rhs.m_valueIndex.begin(), rhs.m_valueIndex.end());
    return *this;
}

template <class T>
template <class D>
CompressedMatrix<T>::CompressedMatrix(const Matrix<D>& rhs):
    Matrix<CompressedMatrix>(rhs.rows(), rhs.columns()),
    m_rowIndex(rhs.rows() + 1)
{
    typename Matrix<D>::const_iterator cur = rhs.begin();
    typename Matrix<D>::const_iterator end = rhs.end();

    size_type currentRow = size_type();
    while (cur != end && *cur != value_type())
    {
        m_valueIndex.push_back(std::make_pair(*cur, cur.column()));
        while (currentRow < cur.row())
        {
            m_rowIndex[currentRow + 1] = m_valueIndex.size() - m_rowIndex[currentRow];
            ++currentRow;
        }
        ++cur;
    }
    m_valueIndex.shrink_to_fit();

    for (; currentRow < this->rows(); ++currentRow)
    {
        m_rowIndex[currentRow + 1] = m_valueIndex.size() - m_rowIndex[currentRow];
    }
}

template <class T>
template <class D>
CompressedMatrix<T>& CompressedMatrix<T>::operator=(const Matrix<D>& rhs)
{
    Matrix<CompressedMatrix>::operator=(rhs);
    this->clear();
    typename CompressedMatrix<D>::const_iterator beg = rhs.begin();
    typename CompressedMatrix<D>::const_iterator end = rhs.end();
    for(; beg != end; beg.nextNonZero())
    {
        *(this)[beg.row()][beg.column()] = *beg;
    }
    return *this;
}

template <class T>
void CompressedMatrix<T>::doClear()
{
    std::fill(m_rowIndex.begin(), m_rowIndex.end(), size_type());
    m_valueIndex.clear();
}

template <class T>
CompressedMatrix<T> CompressedMatrix<T>::doTranspose() const
{
    CompressedMatrix<T> t(this->columns(), this->rows());
    std::vector<std::tuple<T, size_type, size_type>> m_valueColumnRow;
    m_valueColumnRow.resize(m_valueIndex.size());

    for(size_type row = size_type(); row < this->rows(); ++row)
    {
        for(size_type i = m_rowIndex[row]; i < m_rowIndex[row + 1]; ++i)
        {
            m_valueColumnRow[i] = std::make_tuple(m_valueIndex[i].first, m_valueIndex[i].second, row);
        }
    }

    std::stable_sort(m_valueColumnRow.begin(), m_valueColumnRow.end(), [](const std::tuple<T, size_type, size_type>& lhs, const std::tuple<T, size_type, size_type>& rhs)
    {
        return std::get<1>(lhs) < std::get<1>(rhs);
    });
    t.m_valueIndex.resize(m_valueColumnRow.size());
    t.m_rowIndex.resize(this->columns() + 1);
    size_type nextRow = 1;

    for(size_type i = size_type(); i < m_valueColumnRow.size(); ++i)
    {
        t.m_valueIndex[i] = std::make_pair(std::get<0>(m_valueColumnRow[i]), std::get<2>(m_valueColumnRow[i]));

        if(nextRow == std::get<1>(m_valueColumnRow[i]))
        {
            t.m_rowIndex[nextRow] = i;
            ++nextRow;
        }
    }

    t.m_rowIndex[nextRow] = m_valueColumnRow.size();
    return t;
}

#endif
