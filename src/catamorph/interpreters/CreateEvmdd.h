#ifndef NUMERIC_CATAMORPH_CREATEEVMDD_H
#define NUMERIC_CATAMORPH_CREATEEVMDD_H

#include "../Evmdd.h"
#include "../EvmddExpression.h"
#include "../Expression.h"

using Domains = std::map<ID, unsigned int>;
using Ordering = std::map<ID, int>;

template <typename T>
class CreateEvmdd {
private:
    // Evmdd<T> makeConstEvmdd(const float weight);
    // Evmdd<T> makeVarEvmdd(const std::string var, unsigned int domain, int
    // level);
    auto create_evmdd_alg(Domains const &domains, Ordering const &ordering);
    template <typename Tag>
    auto apply(op<Tag, Evmdd<NumericExpression>> const &e);

public:
    Evmdd<T> create_evmdd(Expression const &e, Domains const &domains,
                          Ordering const &ordering);
};

#endif // NUMERIC_CATAMORPH_CREATEEVMDD_H
