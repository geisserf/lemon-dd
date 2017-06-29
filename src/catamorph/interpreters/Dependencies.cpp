#include "Dependencies.h"
#include "../Catamorph.h"
#include "../Factories.h"

template <typename Tag>
std::set<ID> Dependency::join_sets(op<Tag, std::set<ID>> const &op) {
    std::set<ID> out;
    for (auto r : op.rands())
        out.insert(r.begin(), r.end());
    return out;
}

std::set<ID> Dependency::dependencies_alg(expression_r<std::set<ID>> const &e) {
    if (auto *o = Factories::get_as_add(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_mul(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_div(e))
        return Dependency::join_sets(*o);
    if (auto *o = Factories::get_as_sub(e))
        return Dependency::join_sets(*o);
    if (auto *v = Factories::get_as_var(e))
        return {*v};
    return {};
}

std::set<ID> Dependency::dependencies(Expression const &e) {
    return Catamorph::cata<std::set<ID>>(Dependency::dependencies_alg, e);
}
