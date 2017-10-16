#ifndef NUMERIC_CATAMORPH_POLYNOMIAL_H
#define NUMERIC_CATAMORPH_POLYNOMIAL_H

#include "catamorph/expression.h"
#include "catamorph/interpreters/create_evmdd.h"
#include <string>

using Ordering = std::map<std::string, int>;
using Domains = std::map<std::string, unsigned int>;

class Polynomial {
public:
    // Create a polynomial from a string representation of an infix expression
    Polynomial(std::string const &expression);

    // Wraps the expression into a polynomial
    Polynomial(Expression const &e);

    // Evaluates the given polynomial expression regarding the passed
    // environment. Partial evaluation is supported
    Expression evaluate(const Env &environment) const;

    // Returns the constant part of an expression
    Expression get_constant_part() const;

    // Returns an expression which is the polynomial minus a constant
    Expression subtract_constant(Expression const &constant) const;

    // Helper functions to print and show polynomial as a string
    void print() const;
    std::string to_string() const;

    Expression get_expression();

    template <typename M, typename F = std::plus<M>>
    Evmdd<M, F> create_evmdd(Domains const &d, Ordering const &o) const {
        CreateEvmdd<M, F> create;
        return create.create_evmdd(expression, d, o);
    }

private:
    Expression expression;
};

#endif // NUMERIC_CATAMORPH_POLYNOMIAL_H
