#ifndef COMPRESSEDMATRIXTRAITS_H
#define COMPRESSEDMATRIXTRAITS_H

#include "compressedmatrixelementacessor.h"
#include "compressedmatrixiterator.h"
#include "compressedmatrixleftmultiplieradaptor.h"

template <class> class CompressedMatrix;

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

    typedef CompressedMatrixElementAcessor<T, false> acessor;
    typedef CompressedMatrixElementAcessor<T, true> const_acessor;

    typedef CompressedMatrixIterator<T, false> iterator;
    typedef CompressedMatrixIterator<T, true> const_iterator;

    static const bool is_matrix = true;
};

#endif

