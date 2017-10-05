#include "../catamorph/factories.h"
#include "../conditional_effects.h"
#include "../effect_parser.h"
#include "../polynomial.h"

#include "Catch/include/catch.hpp"
#include <iostream>

SCENARIO("Testing basic conditional effect EVMDD creation", "[CE_evmdd]") {
    GIVEN("The conditional effect (a -> b)") {
        std::string e = "a";
        Polynomial p = Polynomial(e);
        ConditionalEffect effect = ConditionalEffect(p.getExpression(), "b", 1);
        ConditionalEffects effects = ConditionalEffects({effect});
        WHEN("We create the EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            Evmdd<Facts, Union> evmdd = effects.create_evmdd(d, o);
            THEN("Evaluation with a = 1 & b = 0 should be {b=1}") {
                ConcreteState state{1, 0};
                Facts result = evmdd.evaluate(state);
                REQUIRE(result.size() == 1);
                REQUIRE(result.begin()->first == "b");
                REQUIRE(result.begin()->second == 1);
            }
        }
    }

    GIVEN("The conditional effect (x & y -> not(v)) &  (not(x) -> z)") {
        Expression e = Factories::land(
            {Factories::var((ID) "x"), Factories::var((ID) "y")});
        ConditionalEffect effect = ConditionalEffect(e, "v", 0);

        Expression e2 = Factories::lnot({Factories::var((ID) "x")});

        ConditionalEffect effect2 = ConditionalEffect(e2, "z", 1);
        ConditionalEffects effects = ConditionalEffects({effect2, effect});
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 2}, {"y", 2}};
            Ordering o = {{"x", 1}, {"y", 2}};
            Evmdd<Facts, Union> evmdd = effects.create_evmdd(d, o);
            THEN("Evaluation with x = 0 & y = 0 should be {z=1}") {
                ConcreteState state{0, 0};
                Facts result = evmdd.evaluate(state);
                REQUIRE(result.size() == 1);
                REQUIRE(result.begin()->first == "z");
                REQUIRE(result.begin()->second == 1);
            }
            THEN("Evaluation with x = 0 & y = 1 should be {z=1}") {
                ConcreteState state{0, 1};
                Facts result = evmdd.evaluate(state);
                REQUIRE(result.size() == 1);
                REQUIRE(result.begin()->first == "z");
                REQUIRE(result.begin()->second == 1);
            }
            THEN("Evaluation with x = 1 & y = 0 should be {}") {
                ConcreteState state{1, 0};
                Facts result = evmdd.evaluate(state);
                REQUIRE(result.empty());
            }
            THEN("Evaluation with x = 1 & y = 1 should be {v=0}") {
                ConcreteState state{1, 1};
                Facts result = evmdd.evaluate(state);
                REQUIRE(result.size() == 1);
                REQUIRE(result.begin()->first == "v");
                REQUIRE(result.begin()->second == 0);
            }
        }
    }

    GIVEN("The conditional effect ([x|y]->!z)") {
        std::string e = "[ x || y ]";
        ConditionalEffect effect =
            ConditionalEffect(Polynomial(e).getExpression(), "z", 0);
        ConditionalEffects effects = ConditionalEffects({effect});
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 2}, {"y", 2}, {"z", 2}};
            Ordering o = {{"z", 1}, {"y", 2}, {"x", 3}};

            Evmdd<Facts, Union> evmdd = effects.create_evmdd(d, o);
            THEN("EVMDD should be") {
                std::stringstream expected;
                expected << "input value: {} nodes:" << std::endl;
                expected << "ID: 3(x)" << std::endl;
                expected << "  w[0]: {}" << std::endl
                         << "    ID: 1(y)" << std::endl;
                expected << "      w[0]: {}" << std::endl
                         << "        ID: 0( )" << std::endl;
                expected << "      w[1]: {z=0}" << std::endl
                         << "        ID: 0( )" << std::endl;

                expected << "  w[1]: {z=0}" << std::endl
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

    GIVEN(
        "The conditional effect:[var5==0 && var6==0 ]->var5==1) & "
        "([var5==0 && "
        "var6==0 ]->var6==1) & ([var5==0 && var6==1 ]->var5==1) & "
        "([var5==0 && "
        "var6==1 ]->var6==2)") {
        std::string e =
            "([[var5==0]&&[var6==0]]->var5==1) & "
            "([[var5==0]&&[var6==0]]->var6==1) & "
            "([[var5==0]&&[var6==1]]->var5==1) & "
            "([[var5==0]&&[var6==1]]->var6==2)";
        EffectParser parser;
        ConditionalEffects effects = parser.parse(e);
        Domains d = {{"var5", 3}, {"var6", 3}};
        Ordering o = {{"var5", 2}, {"var6", 1}};
        THEN("Result should have 4 effects") {
            REQUIRE(effects.getEffects().size() == 4);
        }
        Evmdd<Facts, Union> evmdd = effects.create_evmdd(d, o);
        THEN("EVMDD should be: ") {
            evmdd.print(std::cout);
            // TODO implement test
            REQUIRE(false);
        }
    }
}
