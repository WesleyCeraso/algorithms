#ifndef COMPRESSEDMATRIXITERATORTRAITS_H
#define COMPRESSEDMATRIXITERATORTRAITS_H

#include <algorithm>

template <class> class CompressedMatrix;
template <class, bool> class CompressedMatrixIterator;
template <class> struct matrix_traits;

template <class T, bool Const>
struct matrix_iterator_traits<CompressedMatrixIterator<T, Const>>
{
    typedef typename matrix_traits<CompressedMatrix<T>>::value_type value_type;
    typedef typename matrix_traits<CompressedMatrix<T>>::size_type size_type;
    typedef typename matrix_traits<CompressedMatrix<T>>::difference_type difference_type;
    typedef typename std::conditional<Const, typename std::remove_reference<value_type>::type, typename std::add_lvalue_reference<value_type>::type>::type return_type;
};

#endif

