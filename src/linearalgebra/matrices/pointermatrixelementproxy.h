#ifndef POINTERMATRIXELEMENTPROXY_H
#define POINTERMATRIXELEMENTPROXY_H

#include <vector>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <iomanip>

template <class> class PointerMatrix;

template <class T>
class PointerMatrixItemProxy
{
public:
    PointerMatrixItemProxy(T** pointer);

    operator T&();
    operator T() const;
    PointerMatrixItemProxy& operator=(T* value);
    PointerMatrixItemProxy& operator=(const T value);

private:
    T** m_pointer;
};

template <class T>
PointerMatrixItemProxy<T>::PointerMatrixItemProxy(T** pointer):
    m_pointer(pointer)
{}

template <class T>
PointerMatrixItemProxy<T>::operator T&()
{
    return **m_pointer;
}

template <class T>
PointerMatrixItemProxy<T>::operator T() const
{
    return **m_pointer;
}

template <class T>
PointerMatrixItemProxy<T>& PointerMatrixItemProxy<T>::operator=(T* value)
{
    *m_pointer = value;
    return *this;
}

template <class T>
PointerMatrixItemProxy<T>& PointerMatrixItemProxy<T>::operator=(const T value)
{
    **m_pointer = value;
    return *this;
}

#endif
