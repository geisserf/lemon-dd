#ifndef MULTIPLIES_H
#define MULTIPLIES_H

#include <functional>

template <class T>
struct multiplies {
    multiplies() : annihilator(0), identity(1) {}
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::multiplies<T>()(lhs, rhs);
        return result;
    }
    T annihilator, identity;
    constexpr bool has_identity() const {
        return true;
    }

    constexpr bool has_annihilator() const {
        return true;
    }
};

#endif /* MULTIPLIES_H */