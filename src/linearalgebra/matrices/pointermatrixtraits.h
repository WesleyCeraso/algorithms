#ifndef POINTERMATRIXTRAITS
#define POINTERMATRIXTRAITS

#include "pointermatrixelementacessor.h"
#include "pointermatrixiterator.h"

template <class> class PointerMatrix;

template <class T>
struct matrix_traits<PointerMatrix<T>>
{
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef PointerMatrixElementAcessor<T, false> acessor;
    typedef PointerMatrixElementAcessor<T, true> const_acessor;

    typedef PointerMatrixIterator<T, false> iterator;
    typedef PointerMatrixIterator<T, true> const_iterator;

    static const bool is_matrix = true;
};

#endif

