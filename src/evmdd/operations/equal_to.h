#ifndef EQUAL_TO_H
#define EQUAL_TO_H

#include <functional>

template <class T>
struct equal_to {
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::equal_to<T>()(lhs, rhs);
        return result;
    }
    constexpr bool has_identity() const {
        return false;
    }

    constexpr bool has_annihilator() const {
        return false;
    }
};

#endif /* EQUAL_TO_H */
