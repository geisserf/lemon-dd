#include "../Polynomial.h"
#include "../catamorph/interpreters/Evaluate.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing basic logic expressions and evaluations", "[iverson]") {
    GIVEN("The Expression x==2") {
        Expression e =
            Factories::equals({Factories::var((ID) "x"), Factories::cst(2)});
        Polynomial p(e);
        WHEN("empty environment") {
            Env partial_env = {};
            THEN("Result should be 5") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE(result.toString() == "(== x 2.000000)");
            }
        }

        WHEN("x is 2") {
            Env partial_env = {{"x", 2}};
            Polynomial p(e);
            THEN("Result should be 1") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE(result.toString() == "1.000000");
            }
        }
    }
}
