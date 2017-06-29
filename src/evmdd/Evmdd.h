#ifndef NUMERIC_CATAMORPH_EVMDD_H
#define NUMERIC_CATAMORPH_EVMDD_H

#include "../boost_dependencies/boost/range/adaptors.hpp"
#include "Node.h"

template <typename T>
struct Evmdd_r {
public:
    T label;
    Node<T> child;
};

template <typename T>
struct Evmdd boost::recursive_wrapper<Evmdd_r<T>> {
    using boost::recursive_wrapper<Evmdd_r<Evmdd>>::recursive_wrapper;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
