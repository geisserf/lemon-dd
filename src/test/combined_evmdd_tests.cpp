#include "../combined.h"
#include "../catamorph/factories.h"
#include "../conditional_effects.h"
#include "../polynomial.h"

#include "Catch/include/catch.hpp"
#include <iostream>

SCENARIO("Testing combining numeric anc Conditional effect EVMDDs") {
    GIVEN("CE: x=x+1, Cost x=x+1, Domain x=0..4") {
        std::string cost_function = "x+1";
        Polynomial cost = Polynomial(cost_function);

        Expression cond_effect_1_expresison =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(0)});
        ConditionalEffect effect_1 =
            ConditionalEffect(cond_effect_1_expresison, "x", 1);

        Expression cond_effect_2_expresison =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(1)});
        ConditionalEffect effect_2 =
            ConditionalEffect(cond_effect_2_expresison, "x", 2);

        Expression cond_effect_3_expresison =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(2)});
        ConditionalEffect effect_3 =
            ConditionalEffect(cond_effect_3_expresison, "x", 3);

        Expression cond_effect_4_expresison =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(3)});
        ConditionalEffect effect_4 =
            ConditionalEffect(cond_effect_4_expresison, "x", 4);

        Expression cond_effect_5_expresison =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(4)});
        ConditionalEffect effect_5 =
            ConditionalEffect(cond_effect_5_expresison, "x", 5);

        ConditionalEffects effects = ConditionalEffects(
            {effect_1, effect_2, effect_3, effect_4, effect_5});

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
}
