#ifndef LOGIC_OR_H
#define LOGIC_OR_H

#include <functional>

template <class T>
struct logic_or {
    T operator()(const T& lhs, const T& rhs) const {
        if(!((lhs==0 || lhs==1) && (rhs==0 || rhs==1))){
            throw std::logic_error("Non-boolean operand for logical_or");
        }
        assert((lhs==0 || lhs==1));
        assert((rhs==0 || rhs==1));
        auto result = std::logical_or<T>()(lhs, rhs);
        return result;
    }
};

#endif /* LOGIC_NOT_H */
