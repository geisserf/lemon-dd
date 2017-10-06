#include "../../evmdd/evmdd.h"
#include "../../catamorph/factories.h"
#include "../../conditional_effects.h"
#include "../../polynomial.h"

#include "../Catch/include/catch.hpp"
#include <iostream>

using std::endl;

SCENARIO("Combining numeric and conditional effect EVMDDs", "[combined]") {
    GIVEN("CE: x=x+1, Cost x=x+1, Domain x=0..4") {
        Expression cond_effect_1_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(0)});
        ConditionalEffect effect1 =
            ConditionalEffect(cond_effect_1_expression, "x", 1);

        Expression cond_effect_2_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(1)});
        ConditionalEffect effect2 =
            ConditionalEffect(cond_effect_2_expression, "x", 2);

        Expression cond_effect_3_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(2)});
        ConditionalEffect effect3 =
            ConditionalEffect(cond_effect_3_expression, "x", 3);

        Expression cond_effect_4_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(3)});
        ConditionalEffect effect4 =
            ConditionalEffect(cond_effect_4_expression, "x", 4);

        Expression cond_effect_5_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(4)});
        ConditionalEffect effect5 =
            ConditionalEffect(cond_effect_5_expression, "x", 5);

        Domains d = {{"x", 5}};
        Ordering o = {{"x", 1}};
        ConditionalEffects effects(
            {effect1, effect2, effect3, effect4, effect5});
        Evmdd<Facts, Union> effect_evmdd = effects.create_evmdd(d, o);

        Polynomial cost = Polynomial("x+1");
        Evmdd<int> cost_evmdd = cost.create_evmdd<int>(d, o);

        WHEN("We combine both EVMDDs") {
            ProductFactory<int, Facts, std::plus<int>, Union> factory;
            ProductEvmdd<int, Facts, std::plus<int>, Union> product =
                factory.product(cost_evmdd, effect_evmdd);

            THEN("Evaluation on x=3 should be x=4 with cost 4") {
                ConcreteState state{3};
                auto result = product.evaluate(state);
                int cost = result.first;
                Facts facts = result.second;
                REQUIRE(cost == 4);
                REQUIRE(facts.size() == 1);
                REQUIRE(facts.begin()->first == "x");
                REQUIRE(facts.begin()->second == 4);
            }
        }
    }

    GIVEN(
        "Cost function x+y+z+y*z+1 and the effects: (!x -> !v), (x->u), "
        "((x|y)->!z), ((x&z)->v), (True->w)") {
        std::string cond_effect_1_expression = "[!x]";
        std::string cond_effect_2_expression = "[x]";
        std::string cond_effect_3_expression = "[x || y]";
        std::string cond_effect_4_expression = "[x && z]";
        std::string cond_effect_5_expression = "[1]";
        ConditionalEffect effect1 = ConditionalEffect(
            Polynomial(cond_effect_1_expression).getExpression(), "v", 0);
        ConditionalEffect effect2 = ConditionalEffect(
            Polynomial(cond_effect_2_expression).getExpression(), "u", 1);
        ConditionalEffect effect3 = ConditionalEffect(
            Polynomial(cond_effect_3_expression).getExpression(), "z", 0);
        ConditionalEffect effect4 = ConditionalEffect(
            Polynomial(cond_effect_4_expression).getExpression(), "v", 1);
        ConditionalEffect effect5 = ConditionalEffect(
            Polynomial(cond_effect_5_expression).getExpression(), "w", 1);
        Domains d = {{"x", 2}, {"y", 2}, {"z", 2},
                     {"u", 2}, {"v", 2}, {"w", 2}};
        Ordering o = {{"z", 1}, {"y", 2}, {"x", 3},
                      {"u", 4}, {"v", 5}, {"w", 6}};

        ConditionalEffects effects(
            {effect1, effect2, effect3, effect4, effect5});

        Evmdd<Facts, Union> effect_evmdd = effects.create_evmdd(d, o);

        Polynomial cost = Polynomial("((((x+y)+z)+(y*z))+1)");
        Evmdd<int> cost_evmdd = cost.create_evmdd<int>(d, o);

        WHEN("We combine both EVMDDs") {
            ProductFactory<Facts, int, Union, std::plus<int>> factory;
            auto product = factory.product(effect_evmdd, cost_evmdd);

            THEN("Product should be: ") {
                std::stringstream ss;
                product.print(ss);
                std::stringstream expected;
                expected << "input value: {w=1},1 nodes:" << endl;
                expected << "ID: 7(x)" << endl;
                expected << "  w[0]: {v=0},0" << endl << "    ID: 3(y)" << endl;
                expected << "      w[0]: {},0" << endl
                         << "        ID: 1(z)" << endl;
                expected << "          w[0]: {},0" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: {},1" << endl
                         << "            ID: 0( )" << endl;
                expected << "      w[1]: {z=0},1" << endl
                         << "        ID: 2(z)" << endl;
                expected << "          w[0]: {},0" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: {},2" << endl
                         << "            ID: 0( )" << endl;
                expected << "  w[1]: {u=1 z=0},1" << endl
                         << "    ID: 6(y)" << endl;
                expected << "      w[0]: {},0" << endl
                         << "        ID: 4(z)" << endl;
                expected << "          w[0]: {},0" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: {v=1},1" << endl
                         << "            ID: 0( )" << endl;
                expected << "      w[1]: {},1" << endl
                         << "        ID: 5(z)" << endl;
                expected << "          w[0]: {},0" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: {v=1},2" << endl
                         << "            ID: 0( )" << endl;
                REQUIRE(ss.str() == expected.str());

                // combined_evmdd.print(std::cout);
            }
        }
    }
}
