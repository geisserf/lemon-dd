#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H 

#include "evmdd.h"

#include <map>
#include <memory>
#include <iostream>

using Ordering = std::map<std::string, int>;

template <typename M, typename F = std::plus<M>>
class AbstractFactory {

public:
    // Returns the factory for the given Ordering. Each ordering has exactly one
    // factory which generates and stores all evmdds for this ordering.
    static EvmddFactory<M,F>& get_factory(Ordering const& ordering) {
        if (cache.find(ordering) != cache.end()) {
            std::cout << "Ordering already stored " << std::endl;
            return *(cache.at(ordering));
        }
        cache[ordering] = std::make_unique<EvmddFactory<M,F>>(ordering);
        return *(cache.at(ordering));
    }

private:
    static std::map<Ordering, std::unique_ptr<EvmddFactory<M,F>>> cache;
};

template <typename M, typename F>
std::map<Ordering, std::unique_ptr<EvmddFactory<M,F>>> AbstractFactory<M,F>::cache;







#endif /* ABSTRACT_FACTORY_H */
