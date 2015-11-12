#ifndef DENSEMATRIXITERATORTRAITS_H
#define DENSEMATRIXITERATORTRAITS_H

#include <algorithm>

template <class> class DenseMatrix;
template <class, bool> class DenseMatrixIterator;
template <class> struct matrix_traits;

template <class T, bool Const>
struct matrix_iterator_traits<DenseMatrixIterator<T, Const>>
{
    typedef typename DenseMatrix<T>::value_type value_type;
    typedef typename DenseMatrix<T>::size_type size_type;
    typedef typename DenseMatrix<T>::difference_type difference_type;
    typedef typename std::conditional<Const, typename std::add_const<value_type>::type, typename std::add_lvalue_reference<value_type>::type>::type return_type;
};

#endif

