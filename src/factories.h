#ifndef CATAMORPH_FACTORIES_H
#define CATAMORPH_FACTORIES_H
#include "expression.h"

expression cst(float i) { return expression(i); };

expression var(id id) { return expression(id); };

expression add(std::vector<expression> const &rands) {
  return expression(add_op<expression>{rands});
}

expression sub(std::vector<expression> const &rands) {
  return expression(sub_op<expression>{rands});
}

expression mul(std::vector<expression> const &rands) {
  return expression(mul_op<expression>{rands});
}

expression div(std::vector<expression> const &rands) {
  return expression(div_op<expression>{rands});
}

expression land(std::vector<expression> const &rands) {
  return expression(and_op<expression>{rands});
}

expression equals(std::vector<expression> const &rands) {
  return expression(equals_op<expression>{rands});
}
#endif // CATAMORPH_FACTORIES_H