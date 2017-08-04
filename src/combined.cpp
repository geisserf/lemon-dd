#include "combined.h"
#include "conditional_effects.h"
#include "parser.h"
#include "polynomial.h"

TupleExpression merge::operator()(
    const NumericExpression &first,
    const VariableAssignmentExpression &second) const {
    return TupleExpression{{second, first}};
}

Evmdd<TupleExpression> Combined::create_evmdd(Domains const &d,
                                              Ordering const &o) {
    Evmdd<NumericExpression> cost_evmdd = cost.create_evmdd(d, o);
    Evmdd<VariableAssignmentExpression> ce_evmdd = effects.create_evmdd(d, o);
    EvmddFactory<TupleExpression> factory;

    return factory.apply(cost_evmdd, ce_evmdd, merge());
}
