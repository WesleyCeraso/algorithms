#ifndef COMPRESSEDMATRIX_H
#define COMPRESSEDMATRIX_H

#include "matrix.h"
#include "compressedmatrixiterator.h"
#include "compressedmatrixmultiplylhshandler.h"
#include "compressedmatrixmultiplyrhshandler.h"

template <class T>
struct matrix_traits<CompressedMatrix<T>>
{
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef CompressedMatrixIterator<T, false> iterator;
    typedef CompressedMatrixIterator<T, true> const_iterator;
};

template <class T = double>
class CompressedMatrix : public Matrix<CompressedMatrix<T>>
{
    friend class Matrix<CompressedMatrix<T>>;
    template<typename> friend class CompressedMatrix;
    friend class CompressedMatrixIterator<T, false>;
    friend class CompressedMatrixIterator<T, true>;

    typedef std::vector<std::pair<T, typename CompressedMatrix::size_type>> container_type;
    typedef typename container_type::iterator container_iterator;
    typedef typename container_type::const_iterator container_const_iterator;

public:
    typedef typename matrix_traits<CompressedMatrix>::value_type value_type;
    typedef typename matrix_traits<CompressedMatrix>::size_type size_type;
    typedef typename matrix_traits<CompressedMatrix>::difference_type difference_type;
    typedef typename matrix_traits<CompressedMatrix>::reference reference;
    typedef typename matrix_traits<CompressedMatrix>::const_reference const_reference;
    typedef typename matrix_traits<CompressedMatrix>::pointer pointer;
    typedef typename matrix_traits<CompressedMatrix>::const_pointer const_pointer;

    typedef typename matrix_traits<CompressedMatrix>::iterator iterator;
    typedef typename matrix_traits<CompressedMatrix>::const_iterator const_iterator;

public:
    CompressedMatrix(size_type rows, size_type columns, T value = T());

    CompressedMatrix(const CompressedMatrix& rhs);

    template <class D>
    CompressedMatrix(const CompressedMatrix<D>& rhs);

    template <class D>
    CompressedMatrix(const Matrix<D>& rhs);

    CompressedMatrix& operator=(const CompressedMatrix& rhs);

    template <class D>
    CompressedMatrix& operator=(const CompressedMatrix<D>& rhs);

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
    Matrix<CompressedMatrix>(rows, columns)
{
    assert(rows && columns);
    m_rowIndex.resize(rows + 1);
    if (value != T())
    {
        for (size_type i = 0; i < rows; ++i)
        {
            for (size_type j = 0; j < columns; ++j)
            {
                m_valueIndex.push_back(std::make_pair(value, j));
            }
            m_rowIndex[i + 1] = columns * (i + 1);
        }
    }
}

template <class T>
CompressedMatrix<T>::CompressedMatrix(const CompressedMatrix& rhs):
    Matrix<CompressedMatrix>(rhs.rows(), rhs.columns())
{
    m_rowIndex = rhs.m_rowIndex;
    m_valueIndex = rhs.m_valueIndex;
}

template <class T>
template <class D>
CompressedMatrix<T>::CompressedMatrix(const CompressedMatrix<D>& rhs):
    Matrix<CompressedMatrix>(rhs.rows(), rhs.columns())
{
    m_rowIndex = rhs.m_rowIndex;
    m_valueIndex.resize(rhs.m_valueIndex.size());
    for(size_type i = 0; i < rhs.m_valueIndex.size(); ++i)
    {
        m_valueIndex[i] = rhs.m_valueIndex[i];
    }
}

template <class T>
template <class D>
CompressedMatrix<T>::CompressedMatrix(const Matrix<D>& rhs):
    Matrix<CompressedMatrix>(rhs.rows(), rhs.columns())
{
    typename Matrix<D>::const_iterator beg = rhs.begin();
    typename Matrix<D>::const_iterator end = rhs.end();
    for(; beg != end; beg.nextNonZero())
    {
        *(this)[beg.row()][beg.column()] = *beg;
    }
}

template <class T>
CompressedMatrix<T>& CompressedMatrix<T>::operator=(const CompressedMatrix& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    m_rowIndex = rhs.m_rowIndex;
    m_valueIndex = rhs.m_valueIndex;
    return *this;
}

template <class T>
template <class D>
CompressedMatrix<T>& CompressedMatrix<T>::operator=(const CompressedMatrix<D>& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
    m_rowIndex = rhs.m_rowIndex;
    m_valueIndex.resize(rhs.m_valueIndex.size());
    for(size_type i = 0; i < rhs.m_valueIndex.size(); ++i)
    {
        m_valueIndex[i] = rhs.m_valueIndex[i];
    }
    return *this;
}

template <class T>
template <class D>
CompressedMatrix<T>& CompressedMatrix<T>::operator=(const Matrix<D>& rhs)
{
    assert(this->rows() == rhs.rows() && this->columns() == rhs.columns());
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

    for(size_type row = 0; row < this->rows(); ++row)
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

    for(size_type i = 0; i < m_valueColumnRow.size(); ++i)
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
