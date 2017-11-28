#ifndef NUMERIC_CATAMORPH_CONDITIONALEFFECTS_H
#define NUMERIC_CATAMORPH_CONDITIONALEFFECTS_H

#include "evmdd/monoids/facts.h"
#include "polynomial.h"

#include <string>

using Ordering = std::map<std::string, int>;
using Domains = std::map<std::string, unsigned int>;

template <typename M, typename F>
class Evmdd;

// If first is 1, return second. Otherwise return empty set. Required to
// merge the evmdd for the condition with the evmdd of the effect.
struct keep_if_true {
    Facts operator()(int first, const Facts &second) const;
};

class ConditionalEffect {
public:
    ConditionalEffect(Polynomial const &condition, std::string const &effect,
                      int value)
        : condition(condition), effect(effect), value(value) {}

    // Returns the evmdd representing this conditional effect if (phi) then eff.
    // The resulting evmdd evalutes to eff, iff s satisfies phi, otherwise it
    // evaluates to the empty set.
    Evmdd<Facts, Union> create_evmdd(Domains const &d, Ordering const &o) const;

    std::string to_string() const;

private:
    // Hack: We consider the condition as a numeric expression. This allows us
    // to use iverson bracket expressions to express conditional effects.
    Polynomial condition;
    std::string effect;
    int value;
};

// Convenience method to create evmdd of multiple conditional effects
namespace ConditionalEffects {

// Returns the evmdd representing the union of all conditional effects
Evmdd<Facts, Union> create_evmdd(std::vector<ConditionalEffect> const &effects,
                                 Domains const &d, Ordering const &o);
}

#endif // NUMERIC_CATAMORPH_CONDITIONALEFFECTS_H
