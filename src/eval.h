#ifndef CATAMORPH_EVAL_H
#define CATAMORPH_EVAL_H

#include "catamorphism.h"
#include "dependencies.h"
#include "partial_eval.h"

using env = std::map<id, nb>;

float eval(env const &env, expression const &e) {
  auto reduced = partial_eval(env, e);
  if (auto *i = get_as_cst(reduced.get()))
    return *i;

  std::ostringstream s;
  for (auto const &d : dependencies(reduced))
    s << d << " ";
  throw std::logic_error(s.str());
}

#endif // CATAMORPH_EVAL_H
