#include "../../Catch/include/catch.hpp"

#include "../catamorph/printer.h"
#include "../conditional_effects.h"
#include "../effect_parser.h"

SCENARIO("Testing Effect Parser on conditional effect parsing",
         "[parser][conditional_effect][effect]") {
    GIVEN("The conditional effect (a -> b)") {
        std::string e = "(a -> b)";
        EffectParser parser;
        std::vector<ConditionalEffect> effects = parser.parse(e);
        THEN("Result should have 1 effect") {
            REQUIRE(effects.size() == 1);
            REQUIRE(effects[0].to_string() == "a -> b:=1");
        }
    }

    GIVEN("The conditional effect (x -> y) & (c -> d)") {
        std::string e = "(x ->y) & (c->d)";
        EffectParser parser;
        std::vector<ConditionalEffect> effects = parser.parse(e);
        THEN("Result should have 2 effects") {
            REQUIRE(effects.size() == 2);
            REQUIRE(effects[0].to_string() == "x -> y:=1");
            REQUIRE(effects[1].to_string() == "c -> d:=1");
        }
    }

    GIVEN("The conditional effect ((z | x) -> y:=5) & (c -> !d)") {
        std::string e = "([z ||x]->y==5) & (c->!d)";
        EffectParser parser;
        std::vector<ConditionalEffect> effects = parser.parse(e);
        THEN("Result should have 2 effects") {
            REQUIRE(effects.size() == 2);
            REQUIRE(effects[0].to_string() == "(|| z x) -> y:=5");
            REQUIRE(effects[1].to_string() == "c -> d:=0");
        }
    }

    GIVEN("The conditional effect (x & y -> !v) & (!x -> z)") {
        EffectParser parser;
        auto effects = parser.parse("([x&& y] -> !v) & ([!x] -> z)");
        THEN("Result should have 2 effects") {
            REQUIRE(effects.size() == 2);
            REQUIRE(effects[0].to_string() == "(^ x y) -> v:=0");
            REQUIRE(effects[1].to_string() == "!(x) -> z:=1");
        }
    }
}
