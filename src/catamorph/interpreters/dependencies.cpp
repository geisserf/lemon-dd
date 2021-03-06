#include "dependencies.h"
#include "../catamorph.h"
#include "../factories.h"

template <typename Tag>
std::set<ID> Dependency::join_sets(op<Tag, std::set<ID>> const &op) {
    std::set<ID> out;
    for (auto r : op.rands())
        out.insert(r.begin(), r.end());
    return out;
}

std::set<ID> Dependency::dependencies_alg(expression_r<std::set<ID>> const &e) {
    // arithmetic operators
    if (auto *o = Factories::get_as_add(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_mul(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_div(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_sub(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_abs(e))
        return Dependency::join_sets(*o);
    // logical operators
    if (auto *o = Factories::get_as_and(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_equals(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_or(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_not(e))
        return Dependency::join_sets(*o);
    // equalities
    if (auto *o = Factories::get_as_greater(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_greater_equals(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_less(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_less_equals(e))
        return Dependency::join_sets(*o);
    // base case
    if (auto *v = Factories::get_as_var(e))
        return {*v};
    return {};
}

std::set<ID> Dependency::dependencies(Expression const &e) {
    return Catamorph::cata<std::set<ID>>(Dependency::dependencies_alg, e);
}
