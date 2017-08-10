#include "../combined.h"
#include "../catamorph/factories.h"
#include "../conditional_effects.h"
#include "../polynomial.h"

#include "Catch/include/catch.hpp"
#include <iostream>
using std::endl;
SCENARIO("Testing combining numeric anc Conditional effect EVMDDs",
         "[combined]") {
    GIVEN("CE: x=x+1, Cost x=x+1, Domain x=0..4") {
        std::string cost_function = "x+1";
        Polynomial cost = Polynomial(cost_function);

        Expression cond_effect_1_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(0)});
        ConditionalEffect effect_1 =
            ConditionalEffect(cond_effect_1_expression, "x", 1);

        Expression cond_effect_2_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(1)});
        ConditionalEffect effect_2 =
            ConditionalEffect(cond_effect_2_expression, "x", 2);

        Expression cond_effect_3_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(2)});
        ConditionalEffect effect_3 =
            ConditionalEffect(cond_effect_3_expression, "x", 3);

        Expression cond_effect_4_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(3)});
        ConditionalEffect effect_4 =
            ConditionalEffect(cond_effect_4_expression, "x", 4);

        Expression cond_effect_5_expression =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(4)});
        ConditionalEffect effect_5 =
            ConditionalEffect(cond_effect_5_expression, "x", 5);

        WHEN("Creating EVMDD") {
            Domains d = {{"x", 5}};
            Ordering o = {{"x", 1}};

            Combined combined =
                Combined({effect_1, effect_2, effect_3, effect_4, effect_5},
                         cost_function);
            auto combined_evmdd = combined.create_evmdd(d, o);

            // combined_evmdd.print(std::cout);

            THEN("get Min should be x=1, cost 1") {
                auto result = combined_evmdd.get_min();
                REQUIRE(result[0].value.first.toString() == "{ x=1}");
                REQUIRE(result[0].value.second == 1);
            }

            THEN("Evaluation on x=3 should be x=4 with cost 4") {
                std::vector<int> values_x{3};
                std::map<std::string, std::vector<int>> state{{"x", values_x}};
                auto result = combined_evmdd.evaluate_partial(state);
                REQUIRE(result[0].value.first.toString() == "{ x=4}");
                REQUIRE(result[0].value.second == 4);
            }
        }
    }

    GIVEN(
        "Cost function x+y+z+y*z+1 and the effects: (!x -> !v), (x->u), "
        "((x|y)->!z), ((x&z)->v), (True->w)") {
        std::string cost_function = "((((x+y)+z)+(y*z))+1)";
        std::string cond_effect_1_expression = "(!(x))";
        std::string cond_effect_2_expression = "(x)";
        std::string cond_effect_3_expression = "(x || y)";
        std::string cond_effect_4_expression = "(x && z)";
        std::string cond_effect_5_expression = "(1)";
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

        Combined combined = Combined(
            {effect1, effect2, effect3, effect4, effect5}, cost_function);
        Domains d = {{"x", 2}, {"y", 2}, {"z", 2},
                     {"u", 2}, {"v", 2}, {"w", 2}};
        Ordering o = {{"z", 1}, {"y", 2}, {"x", 3},
                      {"u", 4}, {"v", 5}, {"w", 6}};

        auto combined_evmdd = combined.create_evmdd(d, o);
        WHEN("No evaluation") {
            THEN("evmdd should be: ") {
                std::stringstream ss;
                combined_evmdd.print(ss);
                std::stringstream expected;
                expected << "input value: ({ w=1}, 1) nodes:" << endl;
                expected << "ID: 7(x)" << endl;
                expected << "  w[0]: ({ v=0}, 0)" << endl
                         << "    ID: 3(y)" << endl;
                expected << "      w[0]: ({}, 0)" << endl
                         << "        ID: 1(z)" << endl;
                expected << "          w[0]: ({}, 0)" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: ({}, 1)" << endl
                         << "            ID: 0( )" << endl;
                expected << "      w[1]: ({ z=0}, 1)" << endl
                         << "        ID: 2(z)" << endl;
                expected << "          w[0]: ({}, 0)" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: ({}, 2)" << endl
                         << "            ID: 0( )" << endl;
                expected << "  w[1]: ({ u=1 z=0}, 1)" << endl
                         << "    ID: 6(y)" << endl;
                expected << "      w[0]: ({}, 0)" << endl
                         << "        ID: 4(z)" << endl;
                expected << "          w[0]: ({}, 0)" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: ({ v=1}, 1)" << endl
                         << "            ID: 0( )" << endl;
                expected << "      w[1]: ({}, 1)" << endl
                         << "        ID: 5(z)" << endl;
                expected << "          w[0]: ({}, 0)" << endl
                         << "            ID: 0( )" << endl;
                expected << "          w[1]: ({ v=1}, 2)" << endl
                         << "            ID: 0( )" << endl;
                REQUIRE(ss.str() == expected.str());

                // combined_evmdd.print(std::cout);
            }
        }
    }
}
