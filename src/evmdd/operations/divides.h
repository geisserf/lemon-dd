#ifndef DIVIDES_H
#define DIVIDES_H

#include <functional>

template <class T>
struct divides {
    static const T annihilator = 1, identity;
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::divides<T>()(lhs, rhs);
        return result;
    }
};

#endif /* DIVIDES_H */