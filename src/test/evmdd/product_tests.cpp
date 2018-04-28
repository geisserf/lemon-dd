#include "../../../catamorph/factories.h"
#include "../../../conditional_effects.h"
#include "../../../effect_parser.h"
#include "../../../evmdd/product_evmdd.h"
#include "../../../polynomial.h"
#include "../../../utils/string_utils.h"

#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;
using std::vector;

SCENARIO(
    "Testing construction and evaluation for product of numeric and "
    "conditional effect EVMDD",
    "[evmdd][construction][evaluation][numeric][conditional_effect]") {
    GIVEN("CE: x=x+1, Cost x+1, Domain x=0..4") {
        Domains d = {{"x", 5}};
        Ordering o = {"x"}; //{{"x", 1}};
        std::string ce_string =
            "([x==0]->x==1) & ([x==1]->x==2) & ([x==2]->x==3) & ([x==3]->x==4)";
        EffectParser parser;
        std::vector<ConditionalEffect> effects = parser.parse(ce_string);
        Polynomial cost = Polynomial("x+1");

        WHEN("We combine both EVMDDs") {
            Evmdd<int> cost_evmdd = cost.create_evmdd<int>(d, o);
            Evmdd<Facts, Union> effect_evmdd =
                ConditionalEffects::create_evmdd(effects, d, o);
            auto &factory =
                AbstractProductFactory<Facts, int, Union,
                                       std::plus<int>>::get_factory(o);
            auto product = factory.product(effect_evmdd, cost_evmdd);

            THEN("Evmdd has the correct structure") {
                std::stringstream result;
                product.print(result);
                std::stringstream expected;
                expected << "input: {},1" << endl;
                expected << "x {x=1},0 {x=2},1 {x=3},2 {x=4},3 {},4" << endl;
                REQUIRE(result.str() == expected.str());
            }

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
        Ordering o = {"y", "x"}; // {{"y", 1}, {"x", 2}};
        Polynomial cost = Polynomial("x+2*y+1");
        std::string ce_string =
            "([!x]->!v) & (x->u) & ([x || y] ->!z) & ([1]->w)";
        EffectParser parser;
        std::vector<ConditionalEffect> effects = parser.parse(ce_string);

        WHEN("We combine both EVMDDs") {
            Evmdd<float> cost_evmdd = cost.create_evmdd<float>(d, o);
            Evmdd<Facts, Union> effect_evmdd =
                ConditionalEffects::create_evmdd(effects, d, o);
            auto &factory =
                AbstractProductFactory<Facts, int, Union,
                                       std::plus<int>>::get_factory(o);
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

            THEN("Evaluation on x=1,y=1 should be {w=1, u=1, z=0},4") {
                ConcreteState state{1, 1};
                auto result = product.evaluate(state);
                Facts facts = result.first;
                REQUIRE(facts.size() == 3);
                REQUIRE(StringUtils::fact_printer(facts) == "{u=1 w=1 z=0}");
                REQUIRE(result.second == 4);
            }

            THEN(
                "Partial evaluation on x=0,1 y=1 leads to ({u=1},4),({v=0},3),"
                "({w=1},3), ({z=0},3)") {
                PartialState state{{"x", {0, 1}}, {"y", {1}}};
                auto result = Relaxation::evaluate(product, state);
                std::sort(result.begin(), result.end());
                REQUIRE(result.size() == 4);
                REQUIRE(StringUtils::fact_printer(result[0].first) == "u=1");
                REQUIRE(result[0].second == 4);
                REQUIRE(StringUtils::fact_printer(result[1].first) == "v=0");
                REQUIRE(result[1].second == 3);
                REQUIRE(StringUtils::fact_printer(result[2].first) == "w=1");
                REQUIRE(result[2].second == 3);
                REQUIRE(StringUtils::fact_printer(result[3].first) == "z=0");
                REQUIRE(result[3].second == 3);
            }
        }
    }
}
