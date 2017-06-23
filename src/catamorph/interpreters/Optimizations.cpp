#include "Optimizations.h"

template <typename Tag>
void Optimizations::throw_wrong_parameters(op<Tag, Expression> const &e,
                                           std::string expected) {
  std::ostringstream s;
  s << "wrong number of parameters (expected " << expected << " was "
    << e.rands().size() << ") " << std::endl;
  throw std::logic_error(s.str());
}



void Optimizations::throw_division_by_zero() {
  std::ostringstream s;
  s << "division by 0 exception " << std::endl;
  throw std::logic_error(s.str());
}

template<typename Tag, typename Step>
Expression
Optimizations::optimize_op(op<Tag, Expression> const &e, float neutral,
                           Step step) {

  if (e.rands().size() < 2) {
    throw_wrong_parameters(e, ">=2");
  }

  float res = neutral;

  std::vector<Expression> subs;

  if (auto *i = Factories::get_as_cst(e.rands().front().get())) {
    res = *i;
  } else {
    subs.push_back(e.rands().front());
  }

  for (auto sub = e.rands().begin() + 1; sub != e.rands().end(); sub++) {
    if (auto *i = Factories::get_as_cst(sub->get())) {
      res = step(res, *i);
    } else {
      subs.push_back(*sub);
    }
  }

  if (subs.empty())
    return Factories::cst(res);
  if (res != neutral)
    subs.push_back(Factories::cst(res));
  if (subs.size() == 1)
    return subs.front();
  return Expression(op<Tag, Expression>(subs));
}

template<typename Range>
bool Optimizations::has_zero(const Range &subs) {
  return end(subs) != std::find_if(subs.begin(),subs.end(), [](Expression const &sub) {
      auto *i = Factories::get_as_cst(sub.get());
      return i && *i == 0;
  });
}

Expression Optimizations::opt_add_sub_alg(expression_r<Expression> const &e) {
  if (auto *op = Factories::get_as_add(e))
    return Optimizations::optimize_op(*op, 0, std::plus<float>());
  if (auto *op = Factories::get_as_sub(e))
    return Optimizations::optimize_op(*op, 0, std::minus<float>());
  return e;
}

Expression Optimizations::opt_mul_div_alg(expression_r<Expression> const &e) {
  if (auto *op = Factories::get_as_mul(e)) {
    if (Optimizations::has_zero(op->rands()))
      return Factories::cst(0);
    return Optimizations::optimize_op(*op, 1, std::multiplies<float>());
  }

  if (auto *op = Factories::get_as_div(e)) {
    if (Optimizations::has_zero(op->rands()))
      Optimizations::throw_division_by_zero();
    return Optimizations::optimize_op(*op, 1, std::divides<float>());
  }

  return e;
}

Expression Optimizations::optimize_alg(expression_r<Expression> const &e) {
  return Optimizations::opt_mul_div_alg(Optimizations::opt_add_sub_alg(e).get());
}
