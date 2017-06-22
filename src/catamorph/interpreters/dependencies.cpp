#include "dependencies.h"
#include "../factories.h"
#include "../catamorph.h"


template<typename Tag>
std::set<id> dependency::join_sets(op<Tag, std::set<id>> const &op) {
  std::set<id> out;
  for (auto r : op.rands())
    out.insert(r.begin(), r.end());
  return out;
}

std::set<id>
dependency::dependencies_alg(expression_r<std::set<id>> const &e) {
  if (auto *o = factories::get_as_add(e))
    return dependency::join_sets(*o);
  if (auto *o = factories::get_as_mul(e))
    return dependency::join_sets(*o);
  if (auto *o = factories::get_as_div(e))
    return dependency::join_sets(*o);
  if (auto *o = factories::get_as_sub(e))
    return dependency::join_sets(*o);
  if (auto *v = factories::get_as_var(e))
    return {*v};
  return {};
}

std::set<id> dependency::dependencies(expression const &e) {
  return catamorph::cata<std::set<id>>(dependency::dependencies_alg, e);
}

