#include "create_evmdd.h"
#include "../catamorph.h"
#include <functional>
#include <iostream>

using std::vector;
using std::string;



template <>
auto CreateEvmdd<NumericExpression>::create_evmdd_alg(
    Domains const &domains, Ordering const &ordering) {
    return [&domains, &ordering,
            this](expression_r<NumericEvmdd> const &e) -> NumericEvmdd {
        if (const NBR *cst = Factories::get_as_cst(e)) {
            return factory.make_const_evmdd(NumericExpression(*cst));
        }
        if (auto *var = Factories::get_as_var(e)) {
            std::string name(*var);
            int level = ordering.at(*var);
            int domain = domains.at(*var);
            return factory.make_var_evmdd(level, name, domain);
        }
        if (auto *o = Factories::get_as_add(e)) {
            return apply(o->rands(), std::plus<NumericExpression>());
        }
        if (auto *o = Factories::get_as_sub(e)) {
            return apply(o->rands(), std::minus<NumericExpression>());
        }
        if (auto *o = Factories::get_as_mul(e)) {
            return apply(o->rands(), std::multiplies<NumericExpression>());
        }
        if (auto *o = Factories::get_as_div(e)) {
            return apply(o->rands(), std::divides<NumericExpression>());
        }
        throw std::logic_error("Unknown Operator in Apply");
    };
}

template <>
NumericEvmdd CreateEvmdd<NumericExpression>::create_evmdd(
    Expression const &expr, Domains const &domains, Ordering const &ordering) {
    return Catamorph::cata<NumericEvmdd>(
        [&domains, &ordering,
         this](expression_r<NumericEvmdd> const &expr_r) -> NumericEvmdd {
            return create_evmdd_alg(domains, ordering)(expr_r);
        },
        expr);
}

template class CreateEvmdd<EvmddExpression<float>>;
