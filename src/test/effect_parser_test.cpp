#include "Catch/include/catch.hpp"

#include "../catamorph/printer.h"
#include "../conditional_effects.h"
#include "../effect_parser.h"

SCENARIO("Testing  conditional effect parsing", "[CE_parser]") {
    GIVEN("The conditional effect (a -> b)") {
        std::string e = "(a ->b)";
        WHEN("parsing (a ->b)") {
            EffectParser parser;
            ConditionalEffects effects = parser.parse(e);
            THEN("Result should have 1 effect") {
                REQUIRE(effects.getEffects().size() == 1);
                std::string condition_string =
                    Printer::asString(effects.getEffects()[0].getCondition());
                REQUIRE(condition_string == "a");
                REQUIRE(effects.getEffects()[0].getEffect() == "b");
                REQUIRE(effects.getEffects()[0].getValue() == 1);
            }
        }
    }

    GIVEN("The conditional effect (x -> y) & (c -> d)") {
        std::string e = "(x ->y) & (c->d)";
        WHEN("parsing (x->y) & (c->d)") {
            EffectParser parser;
            ConditionalEffects effects = parser.parse(e);
            THEN("Result should have 2 effects") {
                REQUIRE(effects.getEffects().size() == 2);
                std::string condition_string =
                    Printer::asString(effects.getEffects()[0].getCondition());
                REQUIRE(condition_string == "x");
                REQUIRE(effects.getEffects()[0].getEffect() == "y");
                REQUIRE(effects.getEffects()[0].getValue() == 1);

                condition_string =
                    Printer::asString(effects.getEffects()[1].getCondition());
                REQUIRE(condition_string == "c");
                REQUIRE(effects.getEffects()[1].getEffect() == "d");
                REQUIRE(effects.getEffects()[1].getValue() == 1);
            }
        }
    }
    GIVEN("The conditional effect ((z||x) -> y) & (c -> !d)") {
        std::string e = "([z||x]->y==5) & (c->!d)";
        WHEN("parsing ([z||x]->y==5) & (c->!d)") {
            EffectParser parser;
            ConditionalEffects effects = parser.parse(e);
            THEN("Result should have 2 effects") {
                REQUIRE(effects.getEffects().size() == 2);
                std::string condition_string =
                    Printer::asString(effects.getEffects()[0].getCondition());
                REQUIRE(condition_string == "z||x");
                REQUIRE(effects.getEffects()[0].getEffect() == "y");
                REQUIRE(effects.getEffects()[0].getValue() == 5);

                condition_string =
                    Printer::asString(effects.getEffects()[1].getCondition());
                REQUIRE(condition_string == "c");
                REQUIRE(effects.getEffects()[1].getEffect() == "d");
                REQUIRE(effects.getEffects()[1].getValue() == 0);
            }
        }
    }
}
