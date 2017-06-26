#include "Polynomial.h"
#include "catamorph/Expression.h"
#include "catamorph/Printer.h"
#include "catamorph/interpreters/Dependencies.h"
#include "catamorph/interpreters/Evaluate.h"
#include <string>

Polynomial::Polynomial(std::string expression) {
  this->s_expression = expression;
  std::logic_error("Parser not implemented");
}

Polynomial::Polynomial(const Polynomial &e) { this->expression = e.expression; }

Polynomial::Polynomial(Expression e) {
  this->expression = std::make_shared<Expression>(e);
}

void Polynomial::evaluate(const Env &environment) {
  // We can do better than this casting -> requires proper memory management
  this->expression = std::make_shared<Expression>(
      Evaluate::partial_eval(environment, *(this->expression.get())));
}

NBR Polynomial::getAndSubtractConst() {
  std::cout << "\033[1;33m WARNING! no environment provided! try provding "
               "environment All = 0 for better performance \033[0m"
            << '\n';

  std::set<ID> dependencies =
      Dependency::dependencies(*(this->expression.get()));
  Env environment = Env();
  for (auto f : dependencies) {
    environment.insert(std::pair<ID, NBR>(f, 0));
  }
  return this->getAndSubtractConst(environment);
}

NBR Polynomial::getAndSubtractConst(const Env &environment) {
  Expression res =
      Evaluate::partial_eval(environment, *(this->expression.get()));

  this->expression = std::make_shared<Expression>(
      Factories::sub({*(this->expression.get()), res.get()}));
  return *(Factories::get_as_cst(res.get()));
}

void Polynomial::print() { Printer::print(*(this->expression.get())); }

std::string Polynomial::toString() {
  return Printer::asString(*(this->expression.get()));
}
