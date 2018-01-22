#include "polynomial.h"
#include "catamorph/interpreters/create_evmdd.h"
#include "catamorph/interpreters/dependencies.h"
#include "catamorph/interpreters/evaluate.h"
#include "catamorph/printer.h"
#include "parser.h"

#include <string>

using std::cout;
using std::endl;

Polynomial::Polynomial(std::string const &input) {
    try {
        InfixParser parser;
        expression = parser.parse(input);
    } catch (std::exception const &infix_exception) {
        Parser prefix_parser;
        try {
            expression = prefix_parser.parse(input);
        } catch (std::exception const &prefix_exception) {
            cout << "Parser error: " << endl;
            cout << "Infix parser error: " << infix_exception.what() << endl;
            cout << "Prefix parser error: " << prefix_exception.what() << endl;
        }
    }
}

Polynomial::Polynomial(Expression const &e) {
    expression = e;
}

Expression Polynomial::evaluate(const Env &environment) const {
    return Evaluate::partial_eval(environment, expression);
}

Expression Polynomial::get_constant_part() const {
    std::set<ID> dependencies = Dependency::dependencies(expression);
    Env environment = Env();
    for (auto const &var : dependencies) {
        environment.insert(std::pair<ID, NBR>(var, 0));
    }
    return Evaluate::partial_eval(environment, expression);
}

Expression Polynomial::subtract_constant(Expression const &constant) const {
    return Factories::sub({expression, constant});
}

void Polynomial::print() const {
    Printer::print(expression);
}

std::string Polynomial::to_string() const {
    return Printer::as_string(expression);
}

Expression Polynomial::get_expression() {
    return this->expression;
}
