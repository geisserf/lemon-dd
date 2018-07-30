#include "../../../conditional_effects.h"
#include "../../../effect_parser.h"
#include "../../../evmdd/abstract_factory.h"
#include "../../../utils/string_utils.h"

#include "../../Catch/include/catch.hpp"

using std::endl;

SCENARIO("Testing EVMDDs for conditional effect",
         "[evmdd][conditional_effect]") {
    GIVEN("Evmdd for conditional effect (a -> b)") {
        EffectParser parser;
        auto cond_effects = parser.parse("(a->b)");
        Domains d = {{"a", 2}};
        Ordering o = {"a"};
        Evmdd<Facts, Union> evmdd =
            ConditionalEffects::create_evmdd(cond_effects, d, o);

        THEN("Evmdd has one variable node with 2 edges") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "a {} {b=1}" << endl;
            REQUIRE(result.str() == expected.str());
        }
        THEN("Evaluating state a=0 results in empty set") {
            ConcreteState state{0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state a=1 results in {b=1}") {
            ConcreteState state{1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{b=1}");
        }
    }

    GIVEN("Evmdd for conditional effect (x & y -> !v) & (!x -> z)") {
        EffectParser parser;
        auto cond_effects = parser.parse("([x && y] ->!v) & ([!x] ->z)");
        Domains d = {{"x", 2}, {"y", 2}};
        Ordering o = {"x", "y"};
        Evmdd<Facts, Union> evmdd =
            ConditionalEffects::create_evmdd(cond_effects, d, o);

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
        THEN("Evaluating state !x,!y results in z") {
            ConcreteState state{0, 0};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{z=1}");
        }
        THEN("Evaluating state x,!y results in empty set") {
            ConcreteState state{1, 0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state !x,y results in z") {
            ConcreteState state{0, 1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{z=1}");
        }
        THEN("Evaluating state x,y results in !v") {
            ConcreteState state{1, 1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{v=0}");
        }
    }

    GIVEN("Evmdd for conditional effect ([x|y]->!z)") {
        EffectParser parser;
        auto cond_effects = parser.parse("([x || y] ->!z)");
        Domains d = {{"x", 2}, {"y", 2}};
        Ordering o = {"x", "y"};
        Evmdd<Facts, Union> evmdd =
            ConditionalEffects::create_evmdd(cond_effects, d, o);

        THEN("Reduced evmdd has the correct structure") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "y {} {z=0}" << endl;
            expected << "x {} {z=0}" << endl;
            REQUIRE(result.str() == expected.str());
        }
        THEN("Quasi-reduced evmdd has the correct structure") {
            auto &factory = AbstractFactory<Facts, Union>::get_factory(d, o);
            evmdd = factory.quasi_reduce(evmdd);
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "y {} {z=0}" << endl;
            expected << "x {} {z=0}" << endl;
            expected << "x {} {}" << endl;
            REQUIRE(result.str() == expected.str());
        }
        THEN("Evaluating state !x,!y results in empty set") {
            ConcreteState state{0, 0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state x,!y results in !z") {
            ConcreteState state{1, 0};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{z=0}");
        }
        THEN("Evaluating state !x,y results in !z") {
            ConcreteState state{0, 1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{z=0}");
        }
        THEN("Evaluating state x,y results in !z") {
            ConcreteState state{1, 1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{z=0}");
        }
    }

    GIVEN("Evmdd for conditional effect v5=0 && v6=1 -> v5=1") {
        EffectParser parser;
        auto cond_effects = parser.parse("([[v5==0]&&[v6==1]]->v5==1)");
        Domains d = {{"v5", 2}, {"v6", 3}};
        Ordering o = {"v5", "v6"};
        Evmdd<Facts, Union> evmdd =
            ConditionalEffects::create_evmdd(cond_effects, d, o);
        THEN("Reduced evmdd has the correct structure") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "v6 {} {} {}" << endl;
            expected << "v5 {v5=1} {}" << endl;
            REQUIRE(result.str() == expected.str());
        }
        THEN("Quasi-reduced evmdd has the correct structure") {
            auto &factory = AbstractFactory<Facts, Union>::get_factory(d, o);
            evmdd = factory.quasi_reduce(evmdd);
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
            "([[v5==0]&&[v6==0]]->v5==1) & "
            "([[v5==0]&&[v6==0]]->v6==1) & "
            "([[v5==0]&&[v6==1]]->v5==1) & "
            "([[v5==0]&&[v6==1]]->v6==2)";

        auto cond_effects = parser.parse(e);
        Domains d = {{"v5", 2}, {"v6", 3}};
        Ordering o = {"v5", "v6"};
        Evmdd<Facts, Union> evmdd =
            ConditionalEffects::create_evmdd(cond_effects, d, o);

        THEN("Reduced evmdd has the correct structure") {
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "v6 {} {} {}" << endl;
            expected << "v5 {v5=1 v6=1} {}" << endl;
            expected << "v5 {v5=1 v6=2} {}" << endl;
            REQUIRE(result.str() == expected.str());
        }
        THEN("Quasi-reduced evmdd has the correct structure") {
            auto &factory = AbstractFactory<Facts, Union>::get_factory(d, o);
            evmdd = factory.quasi_reduce(evmdd);
            std::stringstream result;
            evmdd.print(result);
            std::stringstream expected;
            expected << "input: {}" << endl;
            expected << "v6 {} {} {}" << endl;
            expected << "v5 {v5=1 v6=1} {}" << endl;
            expected << "v5 {v5=1 v6=2} {}" << endl;
            expected << "v5 {} {}" << endl;
            REQUIRE(result.str() == expected.str());
        }
        THEN("Evaluating state v5=0,v6=0 results in v5=1 v6=1") {
            ConcreteState state{0, 0};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{v5=1 v6=1}");
        }
        THEN("Evaluating state v5=0,v6=1 results in v5=1 v6=2") {
            ConcreteState state{0, 1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) ==
                    "{v5=1 v6=2}");
        }
        THEN("Evaluating state v5=0,v6=2 results in empty set") {
            ConcreteState state{0, 2};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state v5=1,v6=0 results in empty set") {
            ConcreteState state{1, 0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state v5=1,v6=1 results in empty set") {
            ConcreteState state{1, 1};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state v5=1,v6=2 results in empty set") {
            ConcreteState state{1, 2};
            REQUIRE(evmdd.evaluate(state).empty());
        }
    }
}
