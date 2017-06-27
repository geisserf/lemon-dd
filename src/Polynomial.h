#ifndef NUMERIC_CATAMORPH_POLYNOMIAL_H
#define NUMERIC_CATAMORPH_POLYNOMIAL_H

#include "catamorph/Expression.h"
#include <string>

class Polynomial {
private:
    std::shared_ptr<const Expression> expression;
    std::string s_expression;
    Polynomial() = delete;
    void parseExpressionString();

public:
    // Create a Polynomial from a string representation of an infix expression
    Polynomial(std::string expression);
    Polynomial(const Polynomial &e);
    Polynomial(Expression e);

    // Evaluates the given polynomial expression regarding the passed
    // environment
    // partial evaluation is supported
    void evaluate(const Env &environment);
    // Identifies and subtracts any constant coefficient, by setting all
    // variables
    // to 0.
    NBR getAndSubtractConst();
    // Identifies and subtracts any constant coefficient after application of
    // environment
    // For this all variables must be set.
    NBR getAndSubtractConst(const Env &environment);
    void print();
    std::string toString();
};

#endif // NUMERIC_CATAMORPH_POLYNOMIAL_H
