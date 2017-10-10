#include "../../../evmdd/evmdd.h"
#include "../../../conditional_effects.h"
#include "../../../effect_parser.h"
#include "../../../polynomial.h"

#include "../../Catch/include/catch.hpp"
#include <iostream>

using std::endl;

SCENARIO(
    "Testing construction  for product of numeric and conditional effect "
    "EVMDDs",
    "[product_evmdd_construction]") {
    GIVEN("CE: x=x+1, Cost x+1, Domain x=0..4") {
        Domains d = {{"x", 5}};
        Ordering o = {{"x", 1}};
        std::string ce_string =
            "([x==0]->x==1) & ([x==1]->x==2) & ([x==2]->x==3) & ([x==3]->x==4)";
        EffectParser parser;
        ConditionalEffects effects = parser.parse(ce_string);
        Polynomial cost = Polynomial("x+1");

        WHEN("We combine both EVMDDs") {
            Evmdd<int> cost_evmdd = cost.create_evmdd<int>(d, o);
            Evmdd<Facts, Union> effect_evmdd = effects.create_evmdd(d, o);
            ProductFactory<int, Facts, std::plus<int>, Union> factory;
            ProductEvmdd<int, Facts, std::plus<int>, Union> product =
                factory.product(cost_evmdd, effect_evmdd);

            THEN("Evmdd has the correct structure") {
                std::stringstream result;
                product.print(result);
                std::stringstream expected;
                expected << "input: 1,{}" << endl;
                expected << "x 0,{x=1} 1,{x=2} 2,{x=3} 3,{x=4} 4,{}" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    // Example 3 from AAAI paper
    GIVEN("Cost x+2y+1, effects: (!x -> !v), (x->u), ((x|y)->!z), (True->w)") {
        Domains d = {{"x", 2}, {"y", 2}};
        Ordering o = {{"y", 1}, {"x", 2}};
        Polynomial cost = Polynomial("x+2*y+1");
        std::string ce_string =
            "([!x]->!v) & (x->u) & ([x || y] ->!z) & ([1]->w)";
        EffectParser parser;
        ConditionalEffects effects = parser.parse(ce_string);

        WHEN("We combine both EVMDDs") {
            Evmdd<int> cost_evmdd = cost.create_evmdd<int>(d, o);
            Evmdd<Facts, Union> effect_evmdd = effects.create_evmdd(d, o);
            ProductFactory<Facts, int, Union, std::plus<int>> factory;
            auto product = factory.product(effect_evmdd, cost_evmdd);

            THEN("Evmdd has the correct structure") {
                std::stringstream ss;
                product.print(ss);
                std::stringstream expected;
                expected << "input: {w=1},1" << endl;
                expected << "x {v=0},0 {u=1 z=0},1" << endl;
                expected << "y {},0 {z=0},2" << endl;
                expected << "y {},0 {},2" << endl;
                REQUIRE(ss.str() == expected.str());
            }
        }
    }
}
