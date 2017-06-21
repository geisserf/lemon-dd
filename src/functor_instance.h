#ifndef CATAMORPH_FUNCTOR_INSTANCE_H
#define CATAMORPH_FUNCTOR_INSTANCE_H
#include "accessors.h"
#include "expression.h"

using namespace boost::adaptors;

template <typename A, typename M> auto fmap(M map, expression_r<A> const &e) {

  using B = decltype(map(std::declval<A>()));
  using Out = expression_r<B>;

  if (auto *o = get_as_add(e))
    return Out(add_op<B>(o->rands() | transformed(map)));

  if (auto *o = get_as_sub(e))
    return Out(sub_op<B>(o->rands() | transformed(map)));

  if (auto *o = get_as_mul(e))
    return Out(mul_op<B>(o->rands() | transformed(map)));

  if (auto *o = get_as_div(e))
    return Out(div_op<B>(o->rands() | transformed(map)));

  if (auto *o = get_as_and(e))
    return Out(and_op<B>(o->rands() | transformed(map)));

  if (auto *o = get_as_equals(e))
    return Out(equals_op<B>(o->rands() | transformed(map)));

  if (auto *i = get_as_cst(e))
    return Out(*i);
  if (auto *v = get_as_var(e))
    return Out(*v);

    throw std::logic_error("Missing case in pattern matching");
}

#endif // CATAMORPH_FUNCTOR_INSTANCE_H