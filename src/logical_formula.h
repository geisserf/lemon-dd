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

    Evmdd<bool, std::logical_or<bool>> create_evmdd(Domains const &d,
                                              Ordering const &o) const {
        CreateEvmdd<bool, std::logical_or<bool>> create;
        return create.create_evmdd(expression, d, o);
    }

private:
    Expression expression;
};

#endif /* LOGICAL_FORMULA_H */
