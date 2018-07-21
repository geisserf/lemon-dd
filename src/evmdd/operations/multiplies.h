#ifndef MULTIPLIES_H
#define MULTIPLIES_H

#include <functional>

template <class T>
struct multiplies {
    static const T annihilator = 0, identity = 1;
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::multiplies<T>()(lhs, rhs);
        return result;
    }
};

#endif /* MULTIPLIES_H */