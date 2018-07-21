#ifndef PLUS_H
#define PLUS_H

#include <functional>

template <class T>
struct plus {
    static const T annihilator, identity = 0;
    T operator()(const T &lhs, const T &rhs) const {
        auto result = std::plus<T>()(lhs, rhs);
        return result;
    }
};

#endif /* PLUS_H */