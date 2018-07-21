#ifndef EQUAL_TO_H
#define EQUAL_TO_H

#include <functional>

template <class T>
struct equal_to {
    static const T annihilator, identity;
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::equal_to<T>()(lhs, rhs);
        return result;
    }
};

#endif /* EQUAL_TO_H */