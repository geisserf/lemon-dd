//
// Created by bwright on 22.06.17.
//

#include "iverson.h"
#include "optimizations.h"

expression iverson::opt_equals_alg(expression_r<expression> const &e) {

  if (auto *op = factories::get_as_equals(e)) {
    if (op->rands().size() != 2) {
      std::ostringstream s;
      s << "wrong number of parameters (expected =2 was"<< op->rands().size() << std::endl;
      throw std::logic_error(s.str());

    }

    if (auto *i = factories::get_as_cst(op->rands()[0].get())) {
      if (auto *j = factories::get_as_cst(op->rands()[1].get())) {
        if (*i == *j) {
          return factories::cst(1);
        } else {
          return factories::cst(0);
        }
      }
    }
  }

  return e;
}

expression iverson::opt_or_alg(expression_r<expression> const &e) {
  return e;
}

expression iverson::opt_and_alg(expression_r<expression> const &e) {

  if (auto *op = factories::get_as_and(e)) {
    if (op->rands().size() < 2) {
      std::ostringstream s;
      s << "wrong number of parameters (expected >=2 was"<< op->rands().size() << std::endl;
      throw std::logic_error(s.str());

    }
/*
        for (auto sub = op->rands().begin() + 1; sub != op->rands().end(); sub++) {
            if(auto *i = get_as_cst(sub->get())){

            } else{
                //one element is not yet 1|0
                return e;
            }
        }
*/
    if(auto *i = factories::get_as_cst(op->rands()[0].get())){
      if(auto *j = factories::get_as_cst(op->rands()[1].get())){
        if(*i == 1 && *j == 1){
          return factories::cst(1);
        } else {
          return factories::cst(0);
        }
      }
    }
  }
  return e;
}

expression iverson::iverson_alg(expression_r<expression> const &e) {
  return opt_or_alg(opt_and_alg(opt_equals_alg(e).get()).get());
}
