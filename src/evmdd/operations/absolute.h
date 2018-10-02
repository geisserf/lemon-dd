#ifndef ABSOLUTE_H
#define ABSOLUTE_H 

#include <cmath>

template <class T>
struct absolute {
    // Absolute amount operation disregards rhs and returns abs(lhs).
    // This is a hack to support unary evmdd operations via apply
    T operator()(const T& lhs, const T& /*rhs*/) const {
        return std::abs(lhs);
    }
};

#endif /* ABSOLUTE_H */
