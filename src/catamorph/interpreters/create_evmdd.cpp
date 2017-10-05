#include "create_evmdd.h"
#include "../catamorph.h"

#include <functional>
#include <iostream>

using std::vector;
using std::string;

template <typename M, typename F>
auto CreateEvmdd<M, F>::create_evmdd_alg(Domains const &domains) {
    return [&domains, this](expression_r<Evmdd<M, F>> const &e) -> Evmdd<M, F> {
        if (const NBR *cst = Factories::get_as_cst(e)) {
            return factory.make_const_evmdd(*cst);
        }
        if (auto *var = Factories::get_as_var(e)) {
            string name(*var);
            int domain_size = domains.at(*var);
            vector<M> domain;
            for (int i = 0; i < domain_size; ++i) {
                domain.emplace_back(i);
            }
            return factory.make_var_evmdd(name, domain);
        }
        if (auto *o = Factories::get_as_add(e)) {
            return apply(o->rands(), std::plus<M>());
        }
        if (auto *o = Factories::get_as_sub(e)) {
            return apply(o->rands(), std::minus<M>());
        }
        if (auto *o = Factories::get_as_mul(e)) {
            return apply(o->rands(), std::multiplies<M>());
        }
        if (auto *o = Factories::get_as_div(e)) {
            return apply(o->rands(), std::divides<M>());
        }
        if (auto *o = Factories::get_as_and(e)) {
            return apply(o->rands(), std::logical_and<M>());
        }
        if (auto *o = Factories::get_as_equals(e)) {
            // TODO comparison with tolerance for floats
            return apply(o->rands(), std::equal_to<M>());
        }
        if (auto *o = Factories::get_as_or(e)) {
            return apply(o->rands(), std::logical_or<M>());
        }
         if (auto *o = Factories::get_as_not(e)) {
             return apply(o->rands(), logic_not<M>());
         }

        throw std::logic_error("Unknown Operator in Apply");
    };
}

template <typename M, typename F>
Evmdd<M, F> CreateEvmdd<M, F>::create_evmdd(Expression const &expr,
                                            Domains const &domains,
                                            Ordering const &ordering) {
    factory.set_ordering(ordering);
    return Catamorph::cata<Evmdd<M, F>>(
        [&domains, this](expression_r<Evmdd<M, F>> const &expr_r)
            -> Evmdd<M, F> { return create_evmdd_alg(domains)(expr_r); },
        expr);
}

template class CreateEvmdd<int, std::plus<int>>;
template class CreateEvmdd<float, std::plus<float>>;
template class CreateEvmdd<double, std::plus<double>>;
