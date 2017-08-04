#include "conditional_effects.h"
#include "catamorph/interpreters/create_evmdd.h"
#include "evmdd/evmdd.h"
#include <iostream>
#include <vector>

VariableAssignmentExpression convert_numeric_set::operator()(
    const NumericExpression &first,
    const VariableAssignmentExpression &second) const {
    if (first.value == 1) {
        return VariableAssignmentExpression(second);
    } else {
        std::vector<VariableAssignment> v;
        return VariableAssignmentExpression(v);
    }
    throw std::logic_error("Could not convert");
}

Evmdd<VariableAssignmentExpression> ConditionalEffects::create_evmdd(
    Domains const &d, Ordering const &o) {
    std::vector<Evmdd<VariableAssignmentExpression>> partial;
    CreateEvmdd<NumericExpression> create;
    EvmddFactory<VariableAssignmentExpression> factory;

    for (ConditionalEffect effect : effects) {
        Evmdd<NumericExpression> evmdd =
            create.create_evmdd(effect.getCondition(), d, o);
        // CONVERT
        EvmddFactory<VariableAssignmentExpression> factory;
        Evmdd<VariableAssignmentExpression> evmdd2 =
            factory.make_const_evmdd(VariableAssignmentExpression(
                {{effect.getEffect(), effect.getValue()}}));

        partial.push_back(factory.apply(evmdd, evmdd2, convert_numeric_set()));
    }

    // Union
    Evmdd<VariableAssignmentExpression> result = partial[0];

    for (size_t i = 1; i < partial.size(); ++i) {
        result = factory.apply(result, partial[i],
                               std::plus<VariableAssignmentExpression>());
    }

    return result;
}
