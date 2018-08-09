#ifndef DIVIDES_H
#define DIVIDES_H

#include <functional>

template <class T>
struct divides {
    divides() : identity(1) {}
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::divides<T>()(lhs, rhs);
        return result;
    }
    T identity;
};

#endif /* DIVIDES_H */