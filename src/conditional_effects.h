#ifndef NUMERIC_CATAMORPH_CONDITIONALEFFECTS_H
#define NUMERIC_CATAMORPH_CONDITIONALEFFECTS_H

#include "catamorph/expression.h"
#include "evmdd/evmdd.h"
#include "evmdd/node.h"
#include <string>

using Ordering = std::map<std::string, int>;
using Domains = std::map<std::string, unsigned int>;

// TODO forward declarations

struct convert_numeric_set {
    // TODO this shouldbe monoid<bool>
    Monoid<Facts, Union> operator()(const Monoid<int> &first,
                                    const Monoid<Facts, Union> &second) const;
};

class ConditionalEffect {
public:
    ConditionalEffect(Expression const &condition, std::string const &effect,
                      int value)
        : condition(condition), effect(effect), value(value) {}

    Expression getCondition() {
        return condition;
    }

    std::string getEffect() {
        return effect;
    }

    int getValue() {
        return value;
    }

    Evmdd<Facts, Union> create_evmdd(Domains const &d, Ordering const &o) const;

private:
    Expression condition;
    std::string effect;
    int value;
};

class ConditionalEffects {
public:
    ConditionalEffects() = default;
    ConditionalEffects(std::vector<ConditionalEffect> const &effects)
        : effects(effects) {}

    Evmdd<Facts, Union> create_evmdd(Domains const &d, Ordering const &o) const;

    std::vector<ConditionalEffect> getEffects() {
        return effects;
    }

private:
    std::vector<ConditionalEffect> effects;
};

#endif // NUMERIC_CATAMORPH_CONDITIONALEFFECTS_H
