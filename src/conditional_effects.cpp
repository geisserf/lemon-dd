#include "conditional_effects.h"
#include "evmdd/evmdd.h"

#include <cassert>
#include <vector>

Monoid<Facts, Union> keep_if_true::operator()(
    const BoolMonoid &first, const Monoid<Facts, Union> &second) const {
    if (first.get_value()) {
        return second;
    }
    return Monoid<Facts, Union>::neutral_element();
}

Evmdd<Facts, Union> ConditionalEffect::create_evmdd(Domains const &d,
                                                    Ordering const &o) const {
    // TODO ISSUE #12
    Evmdd<bool, std::logical_or<bool>> condition_evmdd =
        condition.create_evmdd(d, o);
    EvmddFactory<Facts, Union> factory;
    Evmdd<Facts, Union> effect_evmdd =
        factory.make_const_evmdd(Facts{Fact{effect, value}});
    return factory.apply(condition_evmdd, effect_evmdd, keep_if_true());
}

std::string ConditionalEffect::to_string() const {
    std::string result = condition.to_string();
    result += " -> ";
    result += effect + ":=" + std::to_string(value);
    return result;
}

Evmdd<Facts, Union> ConditionalEffects::create_evmdd(Domains const &d,
                                                     Ordering const &o) const {
    std::vector<Evmdd<Facts, Union>> partial;

    for (ConditionalEffect const &effect : effects) {
        partial.push_back(effect.create_evmdd(d, o));
    }

    assert(!partial.empty());

    EvmddFactory<Facts, Union> factory;
    // Union
    Evmdd<Facts, Union> result = partial[0];

    for (size_t i = 1; i < partial.size(); ++i) {
        result = factory.apply(result, partial[i], Union());
    }

    return result;
}
