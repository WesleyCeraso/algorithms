#ifndef POINTERMATRIXITERATORTRAITS_H
#define POINTERMATRIXITERATORTRAITS_H

#include <algorithm>
#include "pointermatrixelementproxy.h"

template <class> class PointerMatrix;
template <class, bool> class PointerMatrixIterator;
template <class> struct matrix_traits;


template <class T, bool Const>
struct matrix_iterator_traits<PointerMatrixIterator<T, Const>>
{
    typedef typename PointerMatrix<T>::value_type value_type;
    typedef typename PointerMatrix<T>::size_type size_type;
    typedef typename PointerMatrix<T>::difference_type difference_type;
    typedef typename std::conditional<Const, const PointerMatrixItemProxy<T>, PointerMatrixItemProxy<T>>::type return_type;
};

#endif

