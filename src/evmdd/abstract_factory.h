#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include "evmdd.h"

#include <iostream>
#include <map>
#include <memory>

using Ordering = std::vector<std::string>;
using Domain = std::map<std::string, unsigned int>;

template <typename L, typename R, typename F, typename G>
using AbstractProductFactory =
    AbstractFactory<std::pair<L, R>, std::pair<F, G>>;

template <typename M, typename F = std::plus<M>>
class AbstractFactory {
public:
    // Returns the factory for the given Ordering. Each ordering has exactly one
    // factory which generates and stores all evmdds for this ordering.
    static EvmddFactory<M, F> &get_factory(Ordering const &ordering,
                                           Domain const& domains) {
        if (cache.find(ordering) != cache.end()) {
            return *(cache.at(ordering));
        }
        cache[ordering] = std::unique_ptr<EvmddFactory<M, F>>(
            new EvmddFactory<M, F>(ordering, domains));
        return *(cache.at(ordering));
    }

    static size_t size() {
        return cache.size();
    }

    // Deletes all factories
    static void reset() {
        cache.clear();
    }

private:
    static std::map<Ordering, std::unique_ptr<EvmddFactory<M, F>>> cache;
};

template <typename M, typename F>
std::map<Ordering, std::unique_ptr<EvmddFactory<M, F>>>
    AbstractFactory<M, F>::cache;

#endif /* ABSTRACT_FACTORY_H */
