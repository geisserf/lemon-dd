#ifndef MAKE_PAIR_H
#define MAKE_PAIR_H

#include <functional>
#include <utility>

template <class T1, class T2>
struct make_pair {
    T1 operator()(const T1 &lhs, const T2 &rhs) const {
        auto result = std::make_pair(lhs, rhs);
        return result;
    }
    constexpr bool has_identity() const {
        return false;
    }

    constexpr bool has_annihilator() const {
        return false;
    }
};

#endif /* MAKE_PAIR_H */
