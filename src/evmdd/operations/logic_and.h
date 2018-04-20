#ifndef LOGIC_AND_H
#define LOGIC_AND_H

#include <functional>
#include <exception>

template <class T>
struct logic_and {
    T operator()(const T& lhs, const T& rhs) const {
        if(!(lhs==0 || lhs==1 || rhs==0 || rhs==1)){
            throw std::logic_error("Non-boolean operands for logical_and");
        }
        auto result = std::logical_and<T>()(lhs, rhs);
        return result;
    }
};

#endif /* LOGIC_AND_H */
