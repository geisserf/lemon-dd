#include "Optimizations.h"
#include "Evaluate.h"
#include "../Expression.h"
#include "Dependencies.h"
#include "Iverson.h"
#include "../Factories.h"
#include "../Catamorph.h"


float Evaluate::eval(Env const &env, Expression const &e) {
  auto reduced = partial_eval(env, e);
  if (auto *i = Factories::get_as_cst(reduced.get()))
    return *i;

  std::ostringstream s;
  for (auto const &d : Dependency::dependencies(reduced))
    s << d << " ";
  throw std::logic_error(s.str());
}

auto Evaluate::partial_eval_alg(Env const &env) {
  return [&env](expression_r<Expression> const &e) -> Expression {
    if (ID const *v = Factories::get_as_var(e)) {
      auto it = env.find(*v);
      if (it != env.end())
        return Factories::cst(it->second);
      return Factories::var(*v);
    }
    return e;
  };
}


Expression Evaluate::partial_eval(Env const &env, Expression const &e) {
  return Catamorph::cata<Expression>(
    [&env](expression_r<Expression> const &e) -> Expression {
      return Optimizations::optimize_alg(Iverson::iverson_alg(Evaluate::partial_eval_alg(env)(e).get()).get());
    },
    e);
}