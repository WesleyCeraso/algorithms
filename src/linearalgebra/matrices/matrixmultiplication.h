#ifndef MATRIXMULTIPLICATION_H
#define MATRIXMULTIPLICATION_H

#include "matrixoperations.h"

template <class M, class N>
class MatrixMultiplication {
public:
    MatrixMultiplication(const M& lhs, const N& rhs):
        m_lhs(lhs),
        m_rhs(rhs)
    {}

    template <class R>
    operator R()
    {
        return MatrixOperations<R>::multiply(m_lhs, m_rhs);
    }

private:
    const M& m_lhs;
    const N& m_rhs;
};

#endif
