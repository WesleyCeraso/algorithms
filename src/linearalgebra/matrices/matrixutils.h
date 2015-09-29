#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

#include <cmath>
#include <cassert>

namespace MatrixUtils
{
template <class T>
T identity(typename T::size_type size, typename T::value_type value = 1)
{
    assert(size);
    T result(size, size);

    for(typename T::size_type i = 0; i < size; ++i)
    {
        result[i][i] = value;
    }

    return result;
}

template <class T>
double euclideanNorm(const T & matrix)
{
    double sum = 0.;
    for (typename T::size_type i = 0; i < matrix.columns(); ++i)
    {
        sum += matrix[0][i] * matrix[0][i];
    }
    return std::sqrt(sum);
}
}

#endif
