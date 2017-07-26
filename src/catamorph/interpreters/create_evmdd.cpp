#include "create_evmdd.h"
#include "../catamorph.h"
#include <functional>
#include <iostream>

using std::vector;
using std::string;

template <>
auto CreateEvmdd<NumericExpression>::create_evmdd_alg(Domains const &domains) {
    return
        [&domains, this](expression_r<NumericEvmdd> const &e) -> NumericEvmdd {
            if (const NBR *cst = Factories::get_as_cst(e)) {
                return factory.make_const_evmdd(NumericExpression(*cst));
            }
            if (auto *var = Factories::get_as_var(e)) {
                string name(*var);
                int domain_size = domains.at(*var);
                vector<NumericExpression> domain;
                for (int i = 0; i < domain_size; ++i) {
                    domain.emplace_back(i);
                }
                return factory.make_var_evmdd(name, domain);
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
            if (auto *o = Factories::get_as_and(e)) {
                return apply(o->rands(), logic_and<NumericExpression>());
            }
            if (auto *o = Factories::get_as_equals(e)) {
                return apply(o->rands(), logic_equals<NumericExpression>());
            }
            if (auto *o = Factories::get_as_or(e)) {
                return apply(o->rands(), logic_or<NumericExpression>());
            }
            if (auto *o = Factories::get_as_not(e)) {
                return apply(o->rands(), logic_not<NumericExpression>());
            }

            throw std::logic_error("Unknown Operator in Apply");
        };
}

template <>
NumericEvmdd CreateEvmdd<NumericExpression>::create_evmdd(
    Expression const &expr, Domains const &domains, Ordering const &ordering) {
    factory.set_ordering(ordering);
    return Catamorph::cata<NumericEvmdd>(
        [&domains, this](expression_r<NumericEvmdd> const &expr_r)
            -> NumericEvmdd { return create_evmdd_alg(domains)(expr_r); },
        expr);
}

template class CreateEvmdd<EvmddExpression<float>>;
