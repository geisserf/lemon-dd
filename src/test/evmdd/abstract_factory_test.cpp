#include "../../Catch/include/catch.hpp"
#include "../../evmdd/abstract_factory.h"

#include <iostream>

using std::endl;

SCENARIO("Testing abstract factory on EVMDDs", "[evmdd][abstract_factory]") {
    AbstractFactory<int>::reset();
    Ordering ab_ordering{"a","b"};
    auto &ab_factory = AbstractFactory<int>::get_factory(ab_ordering);
    auto evmdd_a = ab_factory.make_var_evmdd("a", {0, 1});
    // Terminal node and "a" node
    REQUIRE(ab_factory.size() == 2);
    auto &ab_factory2 = AbstractFactory<int>::get_factory(ab_ordering);
    // Requests the same factory
    REQUIRE(AbstractFactory<int>::size() == 1);
    REQUIRE(ab_factory.size() == ab_factory2.size());

    auto evmdd_a2 = ab_factory2.make_var_evmdd("a", {0, 1});
    REQUIRE(evmdd_a.get_source_node() == evmdd_a2.get_source_node());

    // New ordering should result in a new factory
    Ordering ba_ordering{"b","a"};
    auto &ba_factory = AbstractFactory<int>::get_factory(ba_ordering);
    REQUIRE(AbstractFactory<int>::size() == 2);
    REQUIRE(ba_factory.size() == 1);
}
