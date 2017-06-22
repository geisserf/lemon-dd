#include "optimizations.h"

template <typename Tag>
void optimizations::throw_wrong_parameters(op<Tag, expression> const &e,
                                           std::string expected) {
  std::ostringstream s;
  s << "wrong number of parameters (expected " << expected << " was "
    << e.rands().size() << ") " << std::endl;
  throw std::logic_error(s.str());
}



void optimizations::throw_division_by_zero() {
  std::ostringstream s;
  s << "division by 0 exception " << std::endl;
  throw std::logic_error(s.str());
}

template<typename Tag, typename Step>
expression
optimizations::optimize_op(op<Tag, expression> const &e, float neutral,
                           Step step) {

  if (e.rands().size() < 2) {
    throw_wrong_parameters(e, ">=2");
  }

  float res = neutral;

  std::vector<expression> subs;

  if (auto *i = factories::get_as_cst(e.rands().front().get())) {
    res = *i;
  } else {
    subs.push_back(e.rands().front());
  }

  for (auto sub = e.rands().begin() + 1; sub != e.rands().end(); sub++) {
    if (auto *i = factories::get_as_cst(sub->get())) {
      res = step(res, *i);
    } else {
      subs.push_back(*sub);
    }
  }

  if (subs.empty())
    return factories::cst(res);
  if (res != neutral)
    subs.push_back(factories::cst(res));
  if (subs.size() == 1)
    return subs.front();
  return expression(op<Tag, expression>(subs));
}

template<typename Range>
bool optimizations::has_zero(const Range &subs) {
  return end(subs) != std::find_if(subs.begin(),subs.end(), [](expression const &sub) {
      auto *i = factories::get_as_cst(sub.get());
      return i && *i == 0;
  });
}

expression optimizations::opt_add_sub_alg(expression_r<expression> const &e) {
  if (auto *op = factories::get_as_add(e))
    return optimizations::optimize_op(*op, 0, std::plus<float>());
  if (auto *op = factories::get_as_sub(e))
    return optimizations::optimize_op(*op, 0, std::minus<float>());
  return e;
}

expression optimizations::opt_mul_div_alg(expression_r<expression> const &e) {
  if (auto *op = factories::get_as_mul(e)) {
    if (optimizations::has_zero(op->rands()))
      return factories::cst(0);
    return optimizations::optimize_op(*op, 1, std::multiplies<float>());
  }

  if (auto *op = factories::get_as_div(e)) {
    if (optimizations::has_zero(op->rands()))
      optimizations::throw_division_by_zero();
    return optimizations::optimize_op(*op, 1, std::divides<float>());
  }

  return e;
}

expression optimizations::optimize_alg(expression_r<expression> const &e) {
  return optimizations::opt_mul_div_alg(optimizations::opt_add_sub_alg(e).get());
}
