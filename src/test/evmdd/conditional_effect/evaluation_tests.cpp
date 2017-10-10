#include "../../../conditional_effects.h"
#include "../../../effect_parser.h"
#include "../../../utils/string_utils.h"

#include "../../Catch/include/catch.hpp"

using std::endl;

SCENARIO("Testing concrete evaluation on conditional effect EVMDDs",
         "[effect_evmdd_evaluation]") {
    GIVEN("Evmdd for conditional effect (a -> b)") {
        EffectParser parser;
        auto cond_effects = parser.parse("(a->b)");
        Domains d = {{"a", 2}};
        Ordering o = {{"a", 1}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evaluating state a=0 results in empty set") {
            ConcreteState state{0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state a=1 results in {b=1}") {
            ConcreteState state{1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{b=1}");
        }
    }

    GIVEN("Evmdd for conditional effect (x & y -> !v) & (!x -> z)") {
        EffectParser parser;
        auto cond_effects = parser.parse("([x&&y] ->!v) & ([!x] ->z)");
        Domains d = {{"x", 2}, {"y", 2}};
        Ordering o = {{"x", 1}, {"y", 2}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evaluating state !x,!y results in z") {
            ConcreteState state{0,0};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{z=1}");
        }
        THEN("Evaluating state x,!y results in empty set") {
            ConcreteState state{1,0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state !x,y results in z") {
            ConcreteState state{0,1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{z=1}");
        }
        THEN("Evaluating state x,y results in !v") {
            ConcreteState state{1,1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{v=0}");
        }
    }

    GIVEN("Evmdd for conditional effect ([x|y]->!z)") {
        EffectParser parser;
        auto cond_effects = parser.parse("([x || y] ->!z)");
        Domains d = {{"x", 2}, {"y", 2}};
        Ordering o = {{"x", 1}, {"y", 2}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evaluating state !x,!y results in empty set") {
            ConcreteState state{0,0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state x,!y results in !z") {
            ConcreteState state{1,0};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{z=0}");
        }
        THEN("Evaluating state !x,y results in !z") {
            ConcreteState state{0,1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{z=0}");
        }
        THEN("Evaluating state x,y results in !z") {
            ConcreteState state{1,1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{z=0}");
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
        Ordering o = {{"v5", 1}, {"v6", 2}};
        Evmdd<Facts, Union> evmdd = cond_effects.create_evmdd(d, o);

        THEN("Evaluating state v5=0,v6=0 results in v5=1 v6=1") {
            ConcreteState state{0,0};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{v5=1 v6=1}");
        }
        THEN("Evaluating state v5=0,v6=1 results in v5=1 v6=2") {
            ConcreteState state{0,1};
            REQUIRE(StringUtils::fact_printer(evmdd.evaluate(state)) == "{v5=1 v6=2}");
        }
        THEN("Evaluating state v5=0,v6=2 results in empty set") {
            ConcreteState state{0,2};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state v5=1,v6=0 results in empty set") {
            ConcreteState state{1,0};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state v5=1,v6=1 results in empty set") {
            ConcreteState state{1,1};
            REQUIRE(evmdd.evaluate(state).empty());
        }
        THEN("Evaluating state v5=1,v6=2 results in empty set") {
            ConcreteState state{1,2};
            REQUIRE(evmdd.evaluate(state).empty());
        }
    }
}
