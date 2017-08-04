#ifndef NUMERIC_CATAMORPH_CREATEEVMDD_H
#define NUMERIC_CATAMORPH_CREATEEVMDD_H

#include "../../evmdd/evmdd.h"
#include "../../evmdd/evmdd_expression.h"
#include "../expression.h"

#include <functional>

using Domains = std::map<ID, unsigned int>;

template <typename T>
class CreateEvmdd {
private:
    EvmddFactory<T> factory;

    auto create_evmdd_alg(Domains const &domains);

    template <typename F>
    Evmdd<T> apply(std::vector<Evmdd<T>> const &evmdds, F op) {
        if (evmdds.size() == 1) {
            Evmdd<T> result = evmdds.front();
            // No unary apply implemented equals apply on same evmdd
            return factory.apply(result, result, op);
        }

        Evmdd<T> result = evmdds.front();
        for (size_t i = 1; i < evmdds.size(); ++i) {
            result = factory.apply(result, evmdds[i], op);
        }
        return result;
    }

public:
    Evmdd<T> create_evmdd(Expression const &e, Domains const &domains,
                          Ordering const &ordering);
};

#endif // NUMERIC_CATAMORPH_CREATEEVMDD_H
