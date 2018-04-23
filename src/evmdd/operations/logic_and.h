#ifndef LOGIC_AND_H
#define LOGIC_AND_H

#include <functional>

template <class T>
struct logic_and {
    T operator()(const T& lhs, const T& rhs) const {
        if(!((lhs==0 || lhs==1) && (rhs==0 || rhs==1))){
            throw std::logic_error("Non-boolean operands for logic_and");
        }
        assert((lhs==0 || lhs==1));
        assert((rhs==0 || rhs==1));
        auto result = std::logical_and<T>()(lhs, rhs);
        return result;
    }
};

#endif /* LOGIC_AND_H */
