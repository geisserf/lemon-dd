#ifndef MINUS_H
#define MINUS_H

#include <functional>

template <class T>
struct minus {
    static const T annihilator, identity = 0;
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::minus<T>()(lhs, rhs);
        return result;
    }
};

#endif /* MINUS_H */