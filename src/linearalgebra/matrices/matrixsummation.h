#ifndef MATRIXSUMMATION_H
#define MATRIXSUMMATION_H

#include "matrixoperations.h"

template <class M, class N>
class MatrixSummation {
public:
    MatrixSummation(const M& lhs, const N& rhs):
        m_lhs(lhs),
        m_rhs(rhs)
    {}

    template <class R>
    operator R()
    {
        return MatrixOperations<R>::sum(m_lhs, m_rhs);
    }

private:
    const M& m_lhs;
    const N& m_rhs;
};

#endif
