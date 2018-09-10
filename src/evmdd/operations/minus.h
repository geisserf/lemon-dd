#ifndef MINUS_H
#define MINUS_H

#include <functional>

template <class T>
struct minus {
    minus() : identity(0) {}
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::minus<T>()(lhs, rhs);
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

#endif /* MINUS_H */