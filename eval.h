#ifndef CATAMORPH_EVAL_H
#define CATAMORPH_EVAL_H

#include "catamorphism.h"
#include "dependencies.h"
#include "partial_eval.h"

using env = std::map<id, nb>;

void throw_missing_variables(std::set<id> const &dependencies) {
  std::ostringstream s;
  for (auto const &d : dependencies)
    s << d << " ";
  throw std::logic_error(s.str());
}

float eval(env const &env, expression const &e) {
  auto reduced = partial_eval(env, e);
  if (auto *i = get_as_cst(reduced.get()))
    return *i;
  throw_missing_variables(dependencies(reduced));
}

#endif // CATAMORPH_EVAL_H
