#ifndef PRODUCT_EVMDD_H
#define PRODUCT_EVMDD_H

#include "evmdd.h"

#include <map>

// Product EVMDDs are defined for ProductMonoids. We supply this class for
// specialized evaluation functions for some types of product evmdds.
template <typename L, typename R, typename F, typename G>
using ProductFactory = EvmddFactory<std::pair<L, R>, std::pair<F, G>>;

template <typename L, typename R, typename F, typename G>
using ProductEvmdd = Evmdd<std::pair<L, R>, std::pair<F, G>>;

template <typename T>
using Weight = ProductMonoid<Facts, T, Union, std::plus<T>>;

template <typename T>
struct EvaluationType {
    // Adding Facts F' and cost c to the evaluation type increases the cost of
    // all existing facts F by c, adds F' to F and increases the cheapest_cost
    // by c.
    EvaluationType<T> &operator+=(Weight<T> const &weight);

    // Addition is defined over operator +=
    friend EvaluationType operator+(EvaluationType<T> lhs,
                                    Weight<T> const &rhs) {
        lhs += rhs;
        return lhs;
    }

    std::map<Fact, T> cheapest_facts;
    T cheapest_cost;
};

namespace Relaxation {
// Given a partial state, evaluate the evmdd such that for each fact it's
// cheapest cost among all paths is stored.
template <typename Cost>
std::vector<std::pair<Fact, Cost>> evaluate(
    ProductEvmdd<Facts, Cost, Union, std::plus<Cost>> const &evmdd,
    PartialState const &state);
}

#endif /* PRODUCT_EVMDD_H */
