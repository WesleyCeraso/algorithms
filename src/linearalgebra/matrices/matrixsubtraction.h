#ifndef MATRIXSUBTRACTION_H
#define MATRIXSUBTRACTION_H

#include "matrixoperations.h"

template <class M, class N>
class MatrixSubtraction {
public:
    MatrixSubtraction(const M& lhs, const N& rhs):
        m_lhs(lhs),
        m_rhs(rhs)
    {}

    template <class R>
    operator R()
    {
        return MatrixOperations<R>::subtract(m_lhs, m_rhs);
    }

private:
    const M& m_lhs;
    const N& m_rhs;
};

#endif
