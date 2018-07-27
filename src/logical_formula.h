#ifndef LOGICAL_FORMULA_H
#define LOGICAL_FORMULA_H

#include "catamorph/expression.h"
#include "catamorph/interpreters/create_evmdd.h"

#include <string>

// Class to represent logical formulae. Similar function to Polynomial class.

class Formula {
public:
    Formula(std::string const &expression);

    // Helper functions to print and show formula as a string
    void print() const;
    std::string to_string() const;

    // TODO Deprecated! create_evmdd over bool creates variable nodes where all
    // outgoing edges except the zero edge have value true. This is not what we
    // want for logical formulae.
    Evmdd<bool, std::logical_or<bool>> create_evmdd(Domains const &d,
                                              Ordering const &o) const {
        CreateEvmdd<bool, std::logical_or<bool>> create(o,d);
        return create.create_evmdd(expression, d);
    }

private:
    Expression expression;
};

#endif /* LOGICAL_FORMULA_H */
