#include "../../../evmdd/evmdd.h"
#include "../../../catamorph/factories.h"
#include "../../../conditional_effects.h"
#include "../../../effect_parser.h"
#include "../../../polynomial.h"
#include "../../../utils/string_utils.h"

#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;
using std::vector;

SCENARIO(
    "Testing evaluation for product of numeric and conditional effect EVMDDs",
    "[product_evmdd_evaluation]") {
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
            ProductFactory<Facts, int, Union, std::plus<int>> factory;
            auto product = factory.product(effect_evmdd, cost_evmdd);

            THEN("Evaluation on x=3 should be x=4 with cost 4") {
                ConcreteState state{3};
                auto result = product.evaluate(state);
                int cost = result.second;
                Facts facts = result.first;
                REQUIRE(cost == 4);
                REQUIRE(facts.size() == 1);
                REQUIRE(facts.begin()->first == "x");
                REQUIRE(facts.begin()->second == 4);
            }

            THEN(
                "Partial evaluation on x=0,1,2 leads to (1,{x=1}),(2,{x=2}), "
                "(3,{x=3})") {
                PartialState state{{"x", {0, 1, 2}}};
                auto result = Relaxation::evaluate(product, state);
                std::sort(result.begin(), result.end());
                REQUIRE(result.size() == 3);
                REQUIRE(StringUtils::fact_printer(result[0].first) == "x=1");
                REQUIRE(result[0].second == 1);
                REQUIRE(StringUtils::fact_printer(result[1].first) == "x=2");
                REQUIRE(result[1].second == 2);
                REQUIRE(StringUtils::fact_printer(result[2].first) == "x=3");
                REQUIRE(result[2].second == 3);

            }
        }
    }

    // Example 3 from ICAPS paper
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
        }
    }
}
