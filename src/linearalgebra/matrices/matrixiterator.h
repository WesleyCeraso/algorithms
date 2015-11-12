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

    return_type operator*() const {return static_cast<const I*>(this)->dereference();}
    return_type operator->() const {return static_cast<const I*>(this)->dereference();}

    return_type operator*() {return static_cast<I*>(this)->dereference();}
    return_type operator->() {return static_cast<I*>(this)->dereference();}

    difference_type operator-(const I& rhs) const {return static_cast<const I*>(this)->distance_to(rhs);}

    size_type row() const {return static_cast<const I*>(this)->currentRow();}
    size_type column() const {return static_cast<const I*>(this)->currentColumn();}
    std::tuple<size_type, size_type> rowColumn() const {return std::make_tuple(row(), column());}

protected:
    MatrixIterator() {}
    template <class TT>
    MatrixIterator(const MatrixIterator<TT>&) {}
    template <class TT>
    MatrixIterator& operator=(const MatrixIterator<TT>&) {return *this;}
    template <class TT>
    MatrixIterator(MatrixIterator<TT>&&) {}
    template <class TT>
    MatrixIterator& operator=(MatrixIterator<TT>&&) {return *this;}
};

#endif
