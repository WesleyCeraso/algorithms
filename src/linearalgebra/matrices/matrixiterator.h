#ifndef MATRIXITERATOR_H
#define MATRIXITERATOR_H

#include <iterator>

template <class T>
struct matrix_iterator_traits
{};

template <class I>
class MatrixIterator : public std::iterator<std::random_access_iterator_tag, typename matrix_iterator_traits<I>::value_type, typename matrix_iterator_traits<I>::difference_type>
{
    typedef typename matrix_iterator_traits<I>::value_type value_type;
    typedef typename matrix_iterator_traits<I>::size_type size_type;
    typedef typename matrix_iterator_traits<I>::difference_type difference_type;
    typedef typename matrix_iterator_traits<I>::return_type return_type;

public:
    I& nextNonZero() {static_cast<I*>(this)->incrementNonZero(); return *static_cast<I*>(this);}
    I& previousNonZero() {static_cast<I*>(this)->decrementNonZero(); return *static_cast<I*>(this);}

    I& operator+=(const difference_type diff) {static_cast<I*>(this)->advance(diff); return *static_cast<I*>(this);}
    I& operator++() {static_cast<I*>(this)->increment(); return *static_cast<I*>(this);}
    I operator++(int) {I t(*static_cast<I*>(this)); static_cast<I*>(this)->increment(); return t;}
    I operator+(const difference_type diff) const {I t(*static_cast<const I*>(this)); t += diff; return t;}

    I& operator-=(const difference_type diff) {static_cast<I*>(this)->advance(-diff); return *static_cast<I*>(this);}
    I& operator--() {static_cast<I*>(this)->decrement(); return *static_cast<I*>(this);}
    I operator--(int) {I t(*static_cast<I*>(this)); static_cast<I*>(this)->decrement(); return t;}
    I operator-(const difference_type diff) const {I t(*static_cast<const I*>(this)); t -= diff; return t;}

    bool operator==(const I& rhs) const {return static_cast<const I*>(this)->equal(rhs);}
    bool operator!=(const I& rhs) const {return !static_cast<const I*>(this)->equal(rhs);}

    return_type operator[](const difference_type diff) const {I t(*static_cast<const I*>(this)); t += diff; return t.dereference();}
    return_type operator*() const {return static_cast<const I*>(this)->dereference();}
    return_type operator->() const {return static_cast<const I*>(this)->dereference();}

    return_type operator[](const difference_type diff) {I t(*static_cast<I*>(this)); t += diff; return t.dereference();}
    return_type operator*() {return static_cast<I*>(this)->dereference();}
    return_type operator->() {return static_cast<I*>(this)->dereference();}

    difference_type operator-(const I& rhs) const {return static_cast<const I*>(this)->distance_to(rhs);}

    size_type row() const {return std::get<0>(static_cast<const I*>(this)->rowColumn());}
    size_type column() const {return std::get<1>(static_cast<const I*>(this)->rowColumn());}
    std::tuple<size_type, size_type> rowColumn() const {return static_cast<I*>(this)->rowColumn();}

protected:
    MatrixIterator() {}
    MatrixIterator(const MatrixIterator&) {}
    MatrixIterator& operator=(const MatrixIterator&) {return *this;}
};

#endif
