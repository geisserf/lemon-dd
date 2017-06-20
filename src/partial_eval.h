
#ifndef CATAMORPH_PARTIAL_EVAL_H
#define CATAMORPH_PARTIAL_EVAL_H

#include "catamorphism.h"
#include "factories.h"
#include "iverson.h"
#include "optimizations.h"

using env = std::map<id, nb>;

auto partial_eval_alg(env const &env) {
  return [&env](expression_r<expression> const &e) -> expression {
    if (auto *v = get_as_var(e)) {
      auto it = env.find(*v);
      if (it != env.end())
        return cst(it->second);
      return var(*v);
    }
    return e;
  };
}

expression partial_eval(env const &env, expression const &e) {
  return cata<expression>(
      [&env](expression_r<expression> const &e) -> expression {
        return optimize_alg(iverson_alg(partial_eval_alg(env)(e).get()).get());
      },
      e);
}

#endif // CATAMORPH_PARTIAL_EVAL_H
