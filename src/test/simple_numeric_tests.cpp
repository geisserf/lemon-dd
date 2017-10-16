#include "../polynomial.h"
#include "../catamorph/interpreters/evaluate.h"
#include "../parser.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing basic numeric functions and evaluations", "[numeric]") {
    GIVEN("The Expression 10-5") {
        Polynomial p("10 - 5");
        WHEN("empty environment") {
            Polynomial result(p.evaluate({}));
            THEN("Result should be 5") {
                REQUIRE(result.to_string() == "5");
            }
        }
    }
    GIVEN("The Expression 10+5") {
        Polynomial p("10 + 5");
        WHEN("empty environment") {
            Polynomial result(p.evaluate({}));
            THEN("Result should be 15") {
                REQUIRE(result.to_string() == "15");
            }
        }
    }
    GIVEN("The Expression 10/2") {
        Polynomial p("10 / 2");
        WHEN("empty environment") {
            Polynomial result(p.evaluate({}));
            THEN("Result should be 5") {
                REQUIRE(result.to_string() == "5");
            }
        }
    }

    GIVEN("The Expression 10*2") {
        Polynomial p("10 * 2");
        WHEN("empty environment") {
            Polynomial result(p.evaluate({}));
            THEN("Result should be 20") {
                REQUIRE(result.to_string() == "20");
            }
        }
    }

    GIVEN("The Expression 9/2") {
        Polynomial p("9 / 2");
        WHEN("empty environment") {
            Polynomial result(p.evaluate({}));
            THEN("Result should be 4.5") {
                REQUIRE(result.to_string() == "4.5");
            }
        }
    }

    GIVEN("The Expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
        std::string input = "(+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))";
        Parser parser;
        Polynomial p(parser.parse(input));
        WHEN("x is 1 and y is 2") {
            Env full_env = {{"x", 1}, {"y", 2}};
            THEN("Result should be 8.0") {
                Polynomial result(p.evaluate(full_env));
                REQUIRE(result.to_string() == "8");
            }
        }
        WHEN("y is 2") {
            Env partial_env = {{"y", 2}};
            THEN("Result should be (+ x 7)") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE(result.to_string() == "(+ x 7)");
            }
        }
    }
    GIVEN("The Expression (+ 1 2 (* 0 VAR1 VAR2) (* 1 VAR2 2) (+ 0 VAR1))") {
        std::string input = "(+ 1 2 (* 0 VAR1 VAR2) (* 1 VAR2 2) (+ 0 VAR1))";
        Parser parser;
        Polynomial p(parser.parse(input));
        WHEN("VAR1 is 1 and VAR2 is 2") {
            Env full_env = {{"VAR1", 1}, {"VAR2", 2}};
            THEN("Result should be 8") {
                Polynomial result(p.evaluate(full_env));
                REQUIRE(result.to_string() == "8");
            }
        }
        WHEN("VAR2 is 2") {
            Env partial_env = {{"VAR2", 2}};
            THEN("Result should be (+ VAR1 7)") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE(result.to_string() == "(+ VAR1 7)");
            }
        }
    }
}
