#include "../../evmdd/abstract_factory.h"
#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;

SCENARIO("Testing abstract factory for EVMDDs", "[abstract_factory_test]") {
    Ordering o{{"a",1},{"b",2}};
    auto factory = AbstractFactory<int>::get_factory(o);
    auto evmdd_5 = factory.make_const_evmdd(5);
    auto evmdd_a = factory.make_var_evmdd("a",{0,1});
    auto factory2 = AbstractFactory<int>::get_factory(o);
    auto evmdd_a2 = factory2.make_var_evmdd("a",{0,1});
    evmdd_a.print(std::cout);
    evmdd_a2.print(std::cout);
    REQUIRE(evmdd_a.get_source_node() == evmdd_a2.get_source_node());
   
}
