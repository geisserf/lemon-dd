#include "../catamorph/factories.h"
#include "../conditional_effects.h"
#include "../polynomial.h"
#include "Catch/include/catch.hpp"
#include <iostream>

SCENARIO("Testing basic conditional effect EVMDD creation", "[CE_evmdd]") {
    GIVEN("The conditional effect (a -> b)") {
        std::string e = "a";
        Polynomial p = Polynomial(e);
        ConditionalEffect effect = ConditionalEffect(p.getExpression(), "b", 1);
        WHEN("Evaluating with s: a=1") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            ConditionalEffects effects = ConditionalEffects({effect});
            Evmdd<VariableAssignmentExpression> evmdd =
                effects.create_evmdd(d, o);
            THEN("No error should occur") {
                std::vector<int> values_a{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a}};
                auto result = evmdd.evaluate_partial(state);
                THEN("result shoud be a & b") {
                    REQUIRE(result[0].value.size() == 1);
                    REQUIRE(result[0].value[0].toString() == "b=1");
                }
            }
        }
    }

    GIVEN("The conditional effect (x & y -> not(v)) &  (not(x) -> z)") {
        Expression e = Factories::land(
            {Factories::var((ID) "x"), Factories::var((ID) "y")});
        ConditionalEffect effect = ConditionalEffect(e, "v", 0);

        Expression e2 = Factories::lnot({Factories::var((ID) "x")});
        // Expression e2 = Factories::var((ID) "x");

        ConditionalEffect effect2 = ConditionalEffect(e2, "z", 1);
        WHEN("get_min") {
            Domains d = {{"x", 2}, {"y", 2}, {"v", 2}, {"z", 2}};
            Ordering o = {{"x", 1}, {"y", 2}, {"v", 3}, {"z", 4}};

            ConditionalEffects effects = ConditionalEffects({effect2, effect});
            Evmdd<VariableAssignmentExpression> evmdd =
                effects.create_evmdd(d, o);

            // evmdd.print(std::cout);
            auto result = evmdd.get_min();
            THEN("result should be: ") {
                REQUIRE(result[0].value.size() == 2);
                REQUIRE(result[0].value[0].toString() == "v=0");
                REQUIRE(result[0].value[1].toString() == "z=1");
            }
        }

        WHEN("eavluate x = 1, y=1") {
            Domains d = {{"x", 2}, {"y", 2}, {"v", 2}, {"z", 2}};
            Ordering o = {{"x", 1}, {"y", 2}, {"v", 3}, {"z", 4}};

            ConditionalEffects effects = ConditionalEffects({effect2, effect});
            Evmdd<VariableAssignmentExpression> evmdd =
                effects.create_evmdd(d, o);

            std::vector<int> values_x{1};
            std::vector<int> values_y{1};
            std::map<std::string, std::vector<int>> state{{"x", values_x},
                                                          {"y", values_y}};

            // evmdd.print(std::cout);
            auto result = evmdd.evaluate_partial(state);
            THEN("result should be: ") {
                REQUIRE(result[0].value.size() == 1);
                REQUIRE(result[0].value[0].toString() == "v=0");
            }
        }
        WHEN("eavluate x = 0, y=1") {
            Domains d = {{"x", 2}, {"y", 2}, {"v", 2}, {"z", 2}};
            Ordering o = {{"x", 1}, {"y", 2}, {"v", 3}, {"z", 4}};

            ConditionalEffects effects = ConditionalEffects({effect2, effect});
            Evmdd<VariableAssignmentExpression> evmdd =
                effects.create_evmdd(d, o);

            std::vector<int> values_x{0};
            std::vector<int> values_y{1};
            std::map<std::string, std::vector<int>> state{{"x", values_x},
                                                          {"y", values_y}};

            // evmdd.print(std::cout);
            auto result = evmdd.evaluate_partial(state);
            THEN("result should be: ") {
                REQUIRE(result[0].value.size() == 1);
                REQUIRE(result[0].value[0].toString() == "z=1");
            }
        }
    }

    GIVEN("The conditional effect ([x|y]->!z)") {
        std::string e = "[ x || y ]";
        ConditionalEffect effect =
            ConditionalEffect(Polynomial(e).getExpression(), "z", 0);
        ConditionalEffects effects = ConditionalEffects({effect});
        Domains d = {{"x", 2}, {"y", 2}, {"z", 2}};
        Ordering o = {{"z", 1}, {"y", 2}, {"x", 3}};

        Evmdd<VariableAssignmentExpression> evmdd = effects.create_evmdd(d, o);
        // evmdd.print(std::cout);
        THEN("EVMDD should be") {
            std::stringstream expected;
            expected << "input value: {} nodes:" << std::endl;
            expected << "ID: 3(x)" << std::endl;
            expected << "  w[0]: {}" << std::endl
                     << "    ID: 1(y)" << std::endl;
            expected << "      w[0]: {}" << std::endl
                     << "        ID: 0( )" << std::endl;
            expected << "      w[1]: { z=0}" << std::endl
                     << "        ID: 0( )" << std::endl;

            expected << "  w[1]: { z=0}" << std::endl
                     << "    ID: 2(y)" << std::endl;
            expected << "      w[0]: {}" << std::endl
                     << "        ID: 0( )" << std::endl;
            expected << "      w[1]: {}" << std::endl
                     << "        ID: 0( )" << std::endl;
            std::stringstream ss;
            evmdd.print(ss);

            REQUIRE(ss.str() == expected.str());
        }
    }
}
