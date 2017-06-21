#ifndef CATAMORPH_PRINTER_H
#define CATAMORPH_PRINTER_H
#include "accessors.h"
#include "expression.h"
#include "functor_instance.h"
#include <string>

template <typename Tag>
std::string print_op(op<Tag, std::string> const &e,
                     std::string const &op_repr) {
  return std::string("(") + op_repr + " " +
         boost::algorithm::join(e.rands(), " ") + ")";
}

std::string print_alg(expression_r<std::string> const &e) {
  if (auto *o = get_as_add(e))
    return print_op(*o, "+");
  if (auto *o = get_as_sub(e))
    return print_op(*o, "-");
  if (auto *o = get_as_mul(e))
    return print_op(*o, "*");
  if (auto *o = get_as_div(e))
    return print_op(*o, "/");
  if (auto *o = get_as_equals(e))
    return print_op(*o, "==");
  if (auto *o = get_as_and(e))
    return print_op(*o, "^");
  if (auto *i = get_as_cst(e))
    return std::to_string(*i);
  if (auto *v = get_as_var(e))
    return *v;
  throw std::logic_error("Missing case in pattern matching");
}

#endif // CATAMORPH_PRINTER_H