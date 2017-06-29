#ifndef NUMERIC_CATAMORPH_POLYNOMIAL_H
#define NUMERIC_CATAMORPH_POLYNOMIAL_H

#include "catamorph/Expression.h"

#include <string>

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
    Expression getConstantPart() const;

    // Returns an expression which is the polynomial minus a constant
    Expression subtractConstant(Expression const &constant) const;

    // Helper functions to print and show polynomial as a string
    void print() const;
    std::string toString() const;

private:
    Expression expression;
};

#endif // NUMERIC_CATAMORPH_POLYNOMIAL_H
