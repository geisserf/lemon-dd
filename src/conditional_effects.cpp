#include "conditional_effects.h"
#include "../catamorph/printer.h"
#include "catamorph/interpreters/create_evmdd.h"
#include "utils/math_utils.h"

#include <cassert>
#include <iostream>
#include <vector>

Monoid<Facts, Union> convert_numeric_set::operator()(
    const Monoid<int> &first, const Monoid<Facts, Union> &second) const {
    if (first.get_value() == 1) {
        return Monoid<Facts, Union>(second);
    }
    return Monoid<Facts, Union>::neutral_element();
}

Evmdd<Facts, Union> ConditionalEffect::create_evmdd(Domains const &d,
                                                    Ordering const &o) const {
    // TODO ISSUE #12
    CreateEvmdd<int> create;
    EvmddFactory<Facts, Union> factory;
    Evmdd<int> condition_evmdd = create.create_evmdd(condition, d, o);
    // std::string condition_string = Printer::asString(condition);
    // std::cout << "evmdd for: " << condition_string << std::endl;
    // condition_evmdd.print(std::cout);

    Evmdd<Facts, Union> effect_evmdd =
        factory.make_const_evmdd(Facts{Fact{effect, value}});

    return factory.apply(condition_evmdd, effect_evmdd, convert_numeric_set());
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
