#ifndef DENSEMATRIXTRAITS_H
#define DENSEMATRIXTRAITS_H

#include "densematrixelementacessor.h"
#include "densematrixiterator.h"

template <class> class DenseMatrix;

template <class T>
struct matrix_traits<DenseMatrix<T>>
{
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef DenseMatrixElementAcessor<T, false> acessor;
    typedef DenseMatrixElementAcessor<T, true> const_acessor;

    typedef DenseMatrixIterator<T, false> iterator;
    typedef DenseMatrixIterator<T, true> const_iterator;

    static const bool is_matrix = true;
};

#endif

