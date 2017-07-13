#include "CreateEvmdd.h"
#include "../Catamorph.h"
#include "../EvmddExpression.h"
#include "../Expression.h"
#include <iostream>

template <>
template <typename Tag>
auto CreateEvmdd<NumericExpression>::apply(
    expression_r<Evmdd<NumericExpression>> const &expression,
    op<Tag, Evmdd<NumericExpression>> const &e) {
    if (e.rands().size() < 2) {
        throw std::logic_error("wrong number of parameter expected >2");
    }

    Evmdd<NumericExpression> left_evmdd = e.rands().front();
    for (auto sub = e.rands().begin() + 1; sub != e.rands().end(); sub++) {
        // std::cout<<"execute apply"<<std::endl;
        left_evmdd = left_evmdd.apply(*sub, expression);
    }
    return left_evmdd;
}

template <>
auto CreateEvmdd<NumericExpression>::create_evmdd_alg(
    Domains const &domains, Ordering const &ordering) {
    return
        [&domains, &ordering, this](expression_r<Evmdd<NumericExpression>> const
                                        &e) -> Evmdd<NumericExpression> {
            if (auto *o = Factories::get_as_cst(e)) {
                NumericExpression exp;
                exp.value = *o;
                Evmdd<NumericExpression> result;
                result.evmdd = Evmdd<NumericExpression>::makeConstEvmdd(exp);
                return result;
            }
            if (auto *o = Factories::get_as_var(e)) {
                Evmdd<NumericExpression> result;
                result.evmdd = Evmdd<NumericExpression>::makeVarEvmdd(
                    *o, domains.find(*o)->second, ordering.find(*o)->second);
                return result;
            }
            if (auto *o = Factories::get_as_add(e)) {
                return apply(e, *o);
            }
            if (auto *o = Factories::get_as_sub(e)) {
                return apply(e, *o);
            }
            if (auto *o = Factories::get_as_mul(e)) {
                return apply(e, *o);
            }
            if (auto *o = Factories::get_as_div(e)) {
                return apply(e, *o);
            }

            NumericExpression exp;
            exp.value = 0;
            // not implemented :-)
            Evmdd<NumericExpression> result;
            result.evmdd = Evmdd<NumericExpression>::makeConstEvmdd(exp);
            return result;
        };
}

template <>
Evmdd<NumericExpression> CreateEvmdd<NumericExpression>::create_evmdd(
    Expression const &e, Domains const &domains, Ordering const &ordering) {
    return Catamorph::cata<Evmdd<NumericExpression>>(
        [&domains, &ordering,
         this](expression_r<Evmdd<NumericExpression>> const &e_)
            -> Evmdd<NumericExpression> {
                return create_evmdd_alg(domains, ordering)(e_);
            },
        e);
}

template class CreateEvmdd<EvmddExpression<float>>;
