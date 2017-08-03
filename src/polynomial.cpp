#include "polynomial.h"
#include "catamorph/interpreters/create_evmdd.h"
#include "catamorph/interpreters/dependencies.h"
#include "catamorph/interpreters/evaluate.h"
#include "catamorph/printer.h"
#include "parser.h"

#include <string>

Polynomial::Polynomial(std::string const &input) {
    InfixParser parser;
    expression = parser.parse(input);
}

Polynomial::Polynomial(Expression const &e) {
    expression = e;
}

Expression Polynomial::evaluate(const Env &environment) const {
    return Evaluate::partial_eval(environment, expression);
}

Expression Polynomial::getConstantPart() const {
    std::set<ID> dependencies = Dependency::dependencies(expression);
    Env environment = Env();
    for (auto const &var : dependencies) {
        environment.insert(std::pair<ID, NBR>(var, 0));
    }
    return Evaluate::partial_eval(environment, expression);
}

Expression Polynomial::subtractConstant(Expression const &constant) const {
    return Factories::sub({expression, constant});
}

void Polynomial::print() const {
    Printer::print(expression);
}

std::string Polynomial::toString() const {
    return Printer::asString(expression);
}

Expression Polynomial::getExpression() {
    return this->expression;
}

Evmdd<NumericExpression> Polynomial::create_evmdd(Domains const &d,
                                                  Ordering const &o) {
    CreateEvmdd<NumericExpression> create;
    return create.create_evmdd(expression, d, o);
}
