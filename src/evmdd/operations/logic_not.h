#ifndef LOGIC_NOT_H
#define LOGIC_NOT_H

#include <functional>

template <class T>
struct logic_not {
    // Logical not operation disregards rhs and returns !lhs.
    // This is a hack to support unary evmdd operations via apply
    T operator()(const T& lhs, const T& /*rhs*/) const {
        if(!(lhs==0 || lhs==1)) {
            throw std::logic_error("Non-boolean operand for logical_not");
        }
        assert((lhs==0 || lhs==1));
        auto result = std::logical_not<T>()(lhs);
        return result;
    }
};

#endif /* LOGIC_NOT_H */
