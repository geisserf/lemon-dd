#include "../catamorph/interpreters/evaluate.h"
#include "../polynomial.h"
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
                REQUIRE(result.to_string() == "(== x 2)");
            }
        }

        WHEN("x is 2") {
            Env partial_env = {{"x", 2}};
            Polynomial p(e);
            THEN("Result should be 1") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE(result.to_string() == "1");
            }
        }
    }

    GIVEN("The expression !(x)") {
        Expression e = Factories::lnot({Factories::var((ID) "x")});
        Polynomial p(e);
        WHEN("x =1") {
            Env partial_env = {{"x", 1}};
            THEN("result should be 0") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE(result.to_string() == "0");
            }
        }
    }
}
