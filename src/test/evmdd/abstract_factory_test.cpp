#include "../../evmdd/abstract_factory.h"
#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;

SCENARIO("Testing abstract factory for EVMDDs", "[abstract_factory_test]") {
    AbstractFactory<int>::reset();
    Ordering ab_ordering{{"a",1},{"b",2}};
    auto& ab_factory = AbstractFactory<int>::get_factory(ab_ordering);
    auto evmdd_a = ab_factory.make_var_evmdd("a",{0,1});
    // Terminal node and "a" node
    REQUIRE(ab_factory.size() == 2);
    auto& ab_factory2 = AbstractFactory<int>::get_factory(ab_ordering);
    // Requests the same factory
    REQUIRE(AbstractFactory<int>::size() == 1);
    REQUIRE(ab_factory.size() == ab_factory2.size());

    auto evmdd_a2 = ab_factory2.make_var_evmdd("a",{0,1});
    REQUIRE(evmdd_a.get_source_node() == evmdd_a2.get_source_node());

    // New ordering should result in a new factory
    Ordering ba_ordering{{"a",2},{"b",1}};
    auto& ba_factory = AbstractFactory<int>::get_factory(ba_ordering);
    REQUIRE(AbstractFactory<int>::size() == 2);
    REQUIRE(ba_factory.size() == 1);
}
