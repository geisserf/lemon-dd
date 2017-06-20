#ifndef CATAMORPH_ACCESSORS_H
#define CATAMORPH_ACCESSORS_H
#include "expression.h"

template <typename T> float const *get_as_cst(expression_r<T> const &e) {
  return boost::get<float>(&e);
}

template <typename T> id const *get_as_var(expression_r<T> const &e) {
  return (id *)boost::get<id>(&e);
}

template <typename T> add_op<T> const *get_as_add(expression_r<T> const &e) {
  return boost::get<add_op<T>>(&e);
}

template <typename T> sub_op<T> const *get_as_sub(expression_r<T> const &e) {
  return boost::get<sub_op<T>>(&e);
}

template <typename T> mul_op<T> const *get_as_mul(expression_r<T> const &e) {
  return boost::get<mul_op<T>>(&e);
}

template <typename T> div_op<T> const *get_as_div(expression_r<T> const &e) {
  return boost::get<div_op<T>>(&e);
}

template <typename T> and_op<T> const *get_as_and(expression_r<T> const &e) {
  return boost::get<and_op<T>>(&e);
}

template <typename T>
equals_op<T> const *get_as_equals(expression_r<T> const &e) {
  return boost::get<equals_op<T>>(&e);
}
#endif // CATAMORPH_ACCESSORS_H