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

    constexpr bool has_identity() const {
        return true;
    }

    constexpr bool has_annihilator() const {
        return false;
    }
};

#endif /* DIVIDES_H */