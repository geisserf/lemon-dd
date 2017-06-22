#include "optimizations.h"
#include "evaluate.h"
#include "../expression.h"
#include "dependencies.h"
#include "iverson.h"
#include "../factories.h"
#include "../catamorph.h"


float evaluate::eval(env const &env, expression const &e) {
  auto reduced = partial_eval(env, e);
  if (auto *i = factories::get_as_cst(reduced.get()))
    return *i;

  std::ostringstream s;
  for (auto const &d : dependency::dependencies(reduced))
    s << d << " ";
  throw std::logic_error(s.str());
}

auto evaluate::partial_eval_alg(env const &env) {
  return [&env](expression_r<expression> const &e) -> expression {
    if (id const *v = factories::get_as_var(e)) {
      auto it = env.find(*v);
      if (it != env.end())
        return factories::cst(it->second);
      return factories::var(*v);
    }
    return e;
  };
}


expression evaluate::partial_eval(env const &env, expression const &e) {
  return catamorph::cata<expression>(
    [&env](expression_r<expression> const &e) -> expression {
      return optimizations::optimize_alg(iverson::iverson_alg(evaluate::partial_eval_alg(env)(e).get()).get());
    },
    e);
}