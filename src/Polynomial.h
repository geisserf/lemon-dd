#ifndef NUMERIC_CATAMORPH_POLYNOMIAL_H
#define NUMERIC_CATAMORPH_POLYNOMIAL_H
#include <string>
#include "catamorph/Expression.h"

class Polynomial {
private:
    std::shared_ptr<const Expression> expression;
    std::string s_expression;
    Polynomial() = delete;
public:
    Polynomial(std::string expression);
    Polynomial(const Polynomial& e);
    Polynomial(Expression e);
    void evaluate(const Env& environment);
    void print();
};


#endif //NUMERIC_CATAMORPH_POLYNOMIAL_H
