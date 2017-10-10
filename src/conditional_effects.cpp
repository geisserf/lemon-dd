#include "conditional_effects.h"
#include "evmdd/evmdd.h"

#include <cassert>
#include <vector>

Facts keep_if_true::operator()(int first, const Facts &second) const {
    if (first == 1) {
        return second;
    }
    return Facts{};
}

Evmdd<Facts, Union> ConditionalEffect::create_evmdd(Domains const &d,
                                                    Ordering const &o) const {
    // TODO ISSUE #12
    Evmdd<int> condition_evmdd = condition.create_evmdd<int>(d, o);
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
