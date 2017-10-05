#ifndef LOGIC_NOT_H
#define LOGIC_NOT_H

#include <functional>

template <class T>
struct logic_not {
    // Logical not operation disregards rhs and returns !lhs.
    // This is a hack to support unary evmdd operations via apply
    T operator()(const T& lhs, const T& /*rhs*/) const {
        auto result = std::logical_not<T>()(lhs);
        return result;
    }
};


#endif /* LOGIC_NOT_H */
