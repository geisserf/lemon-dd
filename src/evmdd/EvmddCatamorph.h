#ifndef NUMERIC_CATAMORPH_EVMDD_CATAMORPH_H
#define NUMERIC_CATAMORPH_EVMDD_CATAMORPH_H

#include "Evmdd.h"

class EvmddCatamorp {
public:
    template <typename Out, typename Algebra>
    static Out cata(Algebra f, Evmdd const &evmdd) {
        return f(fmap([f](Evmdd const &e) -> Out { return cata<Out>(f, e); },
                      evmdd.get()));
    }

    template <typename A, typename M>
    static auto fmap(M map, Evmdd_r<A> const &e) {
        using Out = Evmdd_r<A>;

        // Base case Evmdd is constant leading to terminal Node
        if (e.child.variable == nullptr)
            return e;
    };
};

#endif // NUMERIC_CATAMORPH_EVMDD_CATAMORPH_H
