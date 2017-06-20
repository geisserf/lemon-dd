#ifndef CATAMORPH_DEPENDENCIES_H
#define CATAMORPH_DEPENDENCIES_H

#include "catamorphism.h"

template <typename Tag>
std::set<id> join_sets(op<Tag, std::set<id>> const &op) {
  std::set<id> out;
  for (auto r : op.rands())
    out.insert(r.begin(), r.end());
  return out;
}

std::set<id> dependencies_alg(expression_r<std::set<id>> const &e) {
  if (auto *o = get_as_add(e))
    return join_sets(*o);
  if (auto *o = get_as_mul(e))
    return join_sets(*o);
  if (auto *o = get_as_div(e))
    return join_sets(*o);
  if (auto *o = get_as_sub(e))
    return join_sets(*o);
  if (auto *v = get_as_var(e))
    return {*v};
  return {};
}

std::set<id> dependencies(expression const &e) {
  return cata<std::set<id>>(dependencies_alg, e);
}

#endif // CATAMORPH_DEPENDENCIES_H
