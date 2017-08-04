#ifndef NUMERIC_CATAMORPH_COMBINED_H
#define NUMERIC_CATAMORPH_COMBINED_H

#include "catamorph/expression.h"
#include "conditional_effects.h"
#include "evmdd/evmdd.h"
#include "polynomial.h"
#include <string>

class merge {
public:
    TupleExpression operator()(
        const NumericExpression &first,
        const VariableAssignmentExpression &second) const;
};

class Combined {
public:
    Combined() = default;
    Combined(std::vector<ConditionalEffect> const &e, std::string const &c)
        : cost(Polynomial(c)), effects(ConditionalEffects(e)){};

    Evmdd<TupleExpression> create_evmdd(Domains const &d, Ordering const &o);

private:
    Polynomial cost;
    ConditionalEffects effects;
};

#endif // NUMERIC_CATAMORPH_COMBINED_H
