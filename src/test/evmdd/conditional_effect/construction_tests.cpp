#include "../../../conditional_effects.h"
#include "../../../effect_parser.h"

#include "../../Catch/include/catch.hpp"

using std::endl;

SCENARIO("Testing conditional effect EVMDD construction",
         "[effect_evmdd_construction]") {
    GIVEN("Evmdd for conditional effect (a -> b)") {
        EffectParser parser;
        auto cond_effects = parser.parse("(a->b)");
        Domains d = {{"a", 2}};
        Ordering o = {{"a", 1}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evmdd has one variable node with 2 edges") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "a {} {b=1}" << endl;
            REQUIRE(result.str() == expected.str());
        }
    }

    GIVEN("Evmdd for conditional effect (x & y -> !v) & (!x -> z)") {
        EffectParser parser;
        auto cond_effects = parser.parse("([x && y] ->!v) & ([!x] ->z)");
        Domains d = {{"x", 2}, {"y", 2}};
        Ordering o = {{"x", 1}, {"y", 2}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evmdd has the correct structure") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "y {} {}" << endl;
            expected << "x {z=1} {}" << endl;
            expected << "x {z=1} {v=0}" << endl;
            REQUIRE(result.str() == expected.str());
        }
    }

    GIVEN("Evmdd for conditional effect ([x|y]->!z)") {
        EffectParser parser;
        auto cond_effects = parser.parse("([x || y] ->!z)");
        Domains d = {{"x", 2}, {"y", 2}};
        Ordering o = {{"x", 1}, {"y", 2}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evmdd has the correct structure") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "y {} {z=0}" << endl;
            expected << "x {} {z=0}" << endl;
            expected << "x {} {}" << endl;
            REQUIRE(result.str() == expected.str());
        }
    }

    GIVEN("Evmdd for conditional effect v5=0 && v6=1 -> v5=1") {
        EffectParser parser;
        auto cond_effects = parser.parse("([[v5==0]&&[v6==1]]->v5==1)");
        Domains d = {{"v5", 2}, {"v6", 3}};
        Ordering o = {{"v5", 1}, {"v6", 2}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);
        THEN("Evmdd has the correct structure") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "v6 {} {} {}" << endl;
            expected << "v5 {} {}" << endl;
            expected << "v5 {v5=1} {}" << endl;
            REQUIRE(result.str() == expected.str());
        }
    }

    GIVEN("Evmdd for large conditional effect") {
        EffectParser parser;
        std::string e =
            "([[var5==0]&&[var6==0]]->var5==1) & "
            "([[var5==0]&&[var6==0]]->var6==1) & "
            "([[var5==0]&&[var6==1]]->var5==1) & "
            "([[var5==0]&&[var6==1]]->var6==2)";

        auto cond_effects = parser.parse(e);
        Domains d = {{"var5", 2}, {"var6", 3}};
        Ordering o = {{"var5", 1}, {"var6", 2}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evmdd has the correct structure") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "var6 {} {} {}" << endl;
            expected << "var5 {var5=1 var6=1} {}" << endl;
            expected << "var5 {var5=1 var6=2} {}" << endl;
            expected << "var5 {} {}" << endl;
            REQUIRE(result.str() == expected.str());
        }
    }
}
