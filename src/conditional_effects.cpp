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
    Evmdd<int> condition_evmdd = condition.create_evmdd<int>(d, o);
    auto &factory = AbstractFactory<Facts, Union>::get_factory(o,d);
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

Evmdd<Facts, Union> ConditionalEffects::create_evmdd(
    std::vector<ConditionalEffect> const &effects, Domains const &d,
    Ordering const &o) {
    auto &factory = AbstractFactory<Facts, Union>::get_factory(o,d);
    if (effects.empty()) {
        return factory.make_const_evmdd(Facts{});
    }
    std::vector<Evmdd<Facts, Union>> effect_evmdds;

    for (ConditionalEffect const &effect : effects) {
        effect_evmdds.push_back(effect.create_evmdd(d, o));
    }

    Evmdd<Facts, Union> result = effect_evmdds[0];
    // Apply union operator step-wise
    for (size_t i = 1; i < effect_evmdds.size(); ++i) {
        result = factory.apply(result, effect_evmdds[i], Union());
    }

    return result;
}
