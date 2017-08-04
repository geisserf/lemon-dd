#include "conditional_effects.h"
#include "catamorph/interpreters/create_evmdd.h"
#include "evmdd/evmdd.h"
#include "utils/math_utils.h"

#include <cassert>
#include <iostream>
#include <vector>

VariableAssignmentExpression convert_numeric_set::operator()(
    const NumericExpression &first,
    const VariableAssignmentExpression &second) const {
    if (MathUtils::is_equal(first.value, 1)) {
        return VariableAssignmentExpression(second);
    }
    return VariableAssignmentExpression();
}

Evmdd<VariableAssignmentExpression> ConditionalEffect::create_evmdd(
    Domains const &d, Ordering const &o) const {
    // TODO ISSUE #12

    CreateEvmdd<NumericExpression> create;
    EvmddFactory<VariableAssignmentExpression> factory;
    Evmdd<NumericExpression> condition_evmdd =
        create.create_evmdd(condition, d, o);

    Evmdd<VariableAssignmentExpression> effect_evmdd = factory.make_const_evmdd(
        VariableAssignmentExpression({{effect, value}}));
    return factory.apply(condition_evmdd, effect_evmdd, convert_numeric_set());
}

Evmdd<VariableAssignmentExpression> ConditionalEffects::create_evmdd(
    Domains const &d, Ordering const &o) const {
    std::vector<Evmdd<VariableAssignmentExpression>> partial;

    for (ConditionalEffect const &effect : effects) {
        partial.push_back(effect.create_evmdd(d, o));
    }

    assert(!partial.empty());

    EvmddFactory<VariableAssignmentExpression> factory;
    // Union
    Evmdd<VariableAssignmentExpression> result = partial[0];

    for (size_t i = 1; i < partial.size(); ++i) {
        result = factory.apply(result, partial[i],
                               std::plus<VariableAssignmentExpression>());
    }

    return result;
}
