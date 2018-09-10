#ifndef PLUS_H
#define PLUS_H

#include <functional>

template <class T>
struct plus {
    plus() : identity(0) {}
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::plus<T>()(lhs, rhs);
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


#endif /* PLUS_H */