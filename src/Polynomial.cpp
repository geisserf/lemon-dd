#include "Polynomial.h"
#include "catamorph/Expression.h"
#include <string>
#include "catamorph/interpreters/Evaluate.h"
#include "catamorph/Printer.h"

Polynomial::Polynomial(std::string expression) {
  this->s_expression = expression;
  std::logic_error("Parser not implemented");
}

Polynomial::Polynomial(const Polynomial& e) {
  this->expression = e.expression;
}

Polynomial::Polynomial(Expression e) {
  this->expression=std::make_shared<Expression>(e);
}

void Polynomial::evaluate(const Env *environment) {
  //We can do better than this casting -> requires proper memory management
  this->expression=std::make_shared<Expression>(Evaluate::partial_eval(*environment, *(this->expression.get())));
}

void Polynomial::print() {
  Printer::print(*(this->expression.get()));
}
