#ifndef NUMERIC_CATAMORPH_CREATEEVMDD_H
#define NUMERIC_CATAMORPH_CREATEEVMDD_H

#include "../../evmdd/evmdd.h"
#include "../expression.h"

#include <functional>

using Domains = std::map<ID, unsigned int>;

template <typename M, typename F = std::plus<M>>
class CreateEvmdd {
private:
    EvmddFactory<M, F> factory;

    auto create_evmdd_alg(Domains const &domains);

    template <typename OP>
    Evmdd<M, F> apply(std::vector<Evmdd<M, F>> const &evmdds, OP op) {
        Evmdd<M, F> result = evmdds.front();
        if (evmdds.size() == 1) {
            // No unary apply implemented equals apply on same evmdd
            return factory.apply(result, result, op);
        }

        for (size_t i = 1; i < evmdds.size(); ++i) {
            result = factory.apply(result, evmdds[i], op);
        }
        return result;
    }

public:
    Evmdd<M, F> create_evmdd(Expression const &e, Domains const &domains,
                             Ordering const &ordering);
};

#endif // NUMERIC_CATAMORPH_CREATEEVMDD_H
