#ifndef COMPRESSEDMATRIXELEMENTACESSOR_H
#define COMPRESSEDMATRIXELEMENTACESSOR_H

#include <algorithm>
#include <vector>
#include "matrixelementacessor.h"

template <class> class CompressedMatrix;

template <class T, bool Const>
class CompressedMatrixElementAcessor : public MatrixElementAcessor<CompressedMatrixElementAcessor<T, Const>>
{
    friend class MatrixElementAcessor<CompressedMatrixElementAcessor>;
    friend class CompressedMatrixElementAcessor<T, true>;

    typedef typename matrix_element_acessor_traits<CompressedMatrixElementAcessor<T, Const>>::return_type return_type;
    typedef typename matrix_element_acessor_traits<CompressedMatrixElementAcessor<T, Const>>::size_type size_type;
    typedef typename matrix_element_acessor_traits<CompressedMatrixElementAcessor<T, Const>>::value_type value_type;

public:
    CompressedMatrixElementAcessor(CompressedMatrix<T>& matrix, const size_type row):
        MatrixElementAcessor<CompressedMatrixElementAcessor<T, Const>>(row),
        m_matrix(matrix)
    {}

    template <bool C, typename std::enable_if<!C || Const>::type* = nullptr>
    CompressedMatrixElementAcessor(CompressedMatrixElementAcessor<T, C>&& rhs):
        MatrixElementAcessor<CompressedMatrixElementAcessor<T, Const>>(std::move(rhs)),
        m_matrix(rhs.m_matrix)
    {}

protected:
    template <bool C = Const, typename std::enable_if<!C && !Const>::type* = nullptr>
    return_type dereference(const size_type row, const size_type column) const
    {
        auto beg = m_matrix.m_valueIndex.begin() + m_matrix.m_rowIndex[row];
        auto end = m_matrix.m_valueIndex.begin() + m_matrix.m_rowIndex[row + 1];
        auto it = std::lower_bound(beg, end, column,
                                          [](const std::pair<value_type, size_type>& lhs,
                                             const size_type& rhs){
            return lhs.second < rhs;
        });

        if (it == end || it->second != column)
        {
            it = m_matrix.m_valueIndex.insert(it, std::make_pair(value_type(), column));
            for (size_type i = row + 1; i < m_matrix.m_rowIndex.size(); ++i)
            {
                ++m_matrix.m_rowIndex[i];
            }
        }

        return it->first;
    }

    template <bool C = Const, typename std::enable_if<C && Const>::type* = nullptr>
    return_type dereference(const size_type row, const size_type column) const
    {
        auto beg = m_matrix.m_valueIndex.begin() + m_matrix.m_rowIndex[row];
        auto end = m_matrix.m_valueIndex.begin() + m_matrix.m_rowIndex[row + 1];
        auto it = std::lower_bound(beg, end, column,
                                          [](const std::pair<value_type, size_type>& lhs,
                                             const size_type& rhs){
            return lhs.second < rhs;
        });

        if (it == end || it->second != column)
        {
            return s_zero;
        }

        return it->first;
    }

private:
    CompressedMatrixElementAcessor(const CompressedMatrixElementAcessor&) {}
    CompressedMatrixElementAcessor& operator=(const CompressedMatrixElementAcessor&) {return *this;}
    CompressedMatrixElementAcessor& operator=(CompressedMatrixElementAcessor&&) {return *this;}

private:
    CompressedMatrix<T>& m_matrix;
    static const value_type s_zero;
};

template <class T, bool Const>
const typename CompressedMatrixElementAcessor<T, Const>::value_type CompressedMatrixElementAcessor<T, Const>::s_zero = typename CompressedMatrixElementAcessor<T, Const>::value_type();

template <class T, bool Const>
struct matrix_element_acessor_traits<CompressedMatrixElementAcessor<T, Const>>
{
    typedef typename CompressedMatrix<T>::value_type value_type;
    typedef typename CompressedMatrix<T>::size_type size_type;
    typedef typename std::conditional<Const, typename std::add_const<value_type>::type, typename std::add_lvalue_reference<value_type>::type>::type return_type;
};

#endif
