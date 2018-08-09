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
};

#endif /* MINUS_H */