#ifndef NUMERIC_CATAMORPH_CREATEEVMDD_H
#define NUMERIC_CATAMORPH_CREATEEVMDD_H

#include "../Evmdd.h"
#include "../Expression.h"

using Domains = std::map<ID, NBR>;

template <typename T>
class CreateEvmdd {
private:
    Evmdd<T> makeConstEvmdd(const float *weight);
    auto create_evmdd_alg(Domains const &domains);

public:
    Evmdd<T> create_evmdd(Expression const &e, Domains const &domains);
};

#endif // NUMERIC_CATAMORPH_CREATEEVMDD_H
