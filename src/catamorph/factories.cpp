//
// Created by bwright on 22.06.17.
//

#include "factories.h"

expression factories::mul(std::vector<expression> const &rands) {
  return expression(mul_op<expression>{rands});
}

expression factories::sub(std::vector<expression> const &rands) {
  return expression(sub_op<expression>{rands});
}

expression factories::add(std::vector<expression> const &rands) {
  return expression(add_op<expression>{rands});
}

expression factories::var(id id) {
  return expression(id);
}

expression factories::cst(float i) {
  return expression(i);
}

expression factories::div(std::vector<expression> const &rands) {
  return expression(div_op<expression>{rands});
}

expression factories::land(std::vector<expression> const &rands) {
  return expression(and_op<expression>{rands});
}

expression factories::equals(std::vector<expression> const &rands) {
  return expression(equals_op<expression>{rands});
}
