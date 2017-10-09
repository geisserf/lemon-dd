#ifndef NUMERIC_CATAMORPH_CREATEEVMDD_H
#define NUMERIC_CATAMORPH_CREATEEVMDD_H

#include "../../evmdd/evmdd.h"
#include "../catamorph.h"
#include "../expression.h"

#include <functional>
#include <string>
#include <vector>

using Domains = std::map<ID, unsigned int>;

template <typename M, typename F = std::plus<M>>
class CreateEvmdd {
private:
    EvmddFactory<M, F> factory;

    auto create_evmdd_alg(Domains const &domains) {
        return [&domains,
                this](expression_r<Evmdd<M, F>> const &e) -> Evmdd<M, F> {
            if (const NBR *cst = Factories::get_as_cst(e)) {
                return factory.make_const_evmdd(*cst);
            }
            if (auto *var = Factories::get_as_var(e)) {
                std::string name(*var);
                int domain_size = domains.at(*var);
                std::vector<M> domain;
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

    template <typename OP>
    Evmdd<M, F> apply(std::vector<Evmdd<M, F>> const &evmdds, OP op) {
        Evmdd<M, F> result = evmdds.front();
        if (evmdds.size() == 1) {
            // No unary apply implemented equals apply on same evmdd
            return factory.apply(result, result, op);
        }

        for (size_t i = 1; i < evmdds.size(); ++i) {
            result = factory.apply(result, evmdds[i], op);
        }
        return result;
    }

public:
    Evmdd<M, F> create_evmdd(Expression const &expr, Domains const &domains,
                             Ordering const &ordering) {
        factory.set_ordering(ordering);
        return Catamorph::cata<Evmdd<M, F>>(
            [&domains, this](expression_r<Evmdd<M, F>> const &expr_r)
                -> Evmdd<M, F> { return create_evmdd_alg(domains)(expr_r); },
            expr);
    }
};

#endif // NUMERIC_CATAMORPH_CREATEEVMDD_H
