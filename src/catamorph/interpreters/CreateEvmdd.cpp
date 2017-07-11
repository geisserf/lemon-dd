#include "CreateEvmdd.h"
#include "../Catamorph.h"
#include "../EvmddExpression.h"

template <>
auto CreateEvmdd<NumericExpression>::create_evmdd_alg(
    Domains const &domains, Ordering const &ordering) {
    return [&domains, &ordering,
            this](expression_r<Evmdd<NumericExpression>> const &e)
               -> Evmdd<NumericExpression> {
                   if (auto *o = Factories::get_as_cst(e)) {
                       NumericExpression exp;
                       exp.value = *o;
                       return Evmdd<NumericExpression>::makeConstEvmdd(exp);
                   }
                   if (auto *o = Factories::get_as_var(e)) {
                       return Evmdd<NumericExpression>::makeVarEvmdd(
                           *o, domains.find(*o)->second,
                           ordering.find(*o)->second);
                   }
                   NumericExpression exp;
                   exp.value = 0;
                   // not implemented :-)
                   return Evmdd<NumericExpression>::makeConstEvmdd(exp);
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
