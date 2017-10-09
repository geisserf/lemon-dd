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
                REQUIRE(effects.getEffects()[0].to_string() == "a -> b:=1");
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
                REQUIRE(effects.getEffects()[0].to_string() == "x -> y:=1");
                REQUIRE(effects.getEffects()[1].to_string() == "c -> d:=1");
            }
        }
    }

    GIVEN("The conditional effect ((z||x) -> y:=5) & (c -> !d)") {
        std::string e = "([z||x]->y==5) & (c->!d)";
        WHEN("parsing ([z||x]->y==5) & (c->!d)") {
            EffectParser parser;
            ConditionalEffects effects = parser.parse(e);
            THEN("Result should have 2 effects") {
                REQUIRE(effects.getEffects().size() == 2);
                REQUIRE(effects.getEffects()[0].to_string() == "z||x -> y:=5");
                REQUIRE(effects.getEffects()[1].to_string() == "c -> d:=0");
            }
        }
    }

    GIVEN("The conditional effect (x & y -> !v) & (!x -> z)") {
        EffectParser parser;
        auto effects = parser.parse("([x&&y] -> !v) & ([!x] -> z)");
        THEN("Result should have 2 effects") {
            REQUIRE(effects.getEffects().size() == 2);
            REQUIRE(effects.getEffects()[0].to_string() == "(^ x y) -> v:=0");
            REQUIRE(effects.getEffects()[1].to_string() == "!x -> z:=1");
        }
    }
}
