#ifndef PRODUCT_EVMDD_H
#define PRODUCT_EVMDD_H

#include "evmdd.h"

template <typename L, typename R, typename F, typename G>
using ProductEvmdd = Evmdd<std::pair<L, R>, std::pair<F, G>>;

// TODO compile time error message if we specify ProductFactory over L,R,F,G
// but call product(EVMDD<R>, EVMDD<L>)
template <typename L, typename R, typename F, typename G>
class ProductFactory {
public:
    ProductEvmdd<L, R, F, G> product(Evmdd<L, F> const &l,
                                     Evmdd<R, G> const &r) {
        return factory.apply(l, r, std::make_pair<L,R>);
    }

private:
    EvmddFactory<std::pair<L, R>, std::pair<F, G>> factory;
};

#endif /* PRODUCT_EVMDD_H */
