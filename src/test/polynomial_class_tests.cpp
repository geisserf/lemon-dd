#include "../polynomial.h"
#include "../catamorph/interpreters/evaluate.h"
#include "../parser.h"
#include "Catch/include/catch.hpp"

#include <exception>
#include <iostream>
#include <string>

SCENARIO("Testing Polynomial Input as prefix and infix string", "[poly]") {
    GIVEN("An Infix Expression") {
        std::string input = "(a+b)";
        Polynomial p(input);
        THEN("Polynomial is a+b") {
            REQUIRE("(+ a b)" == p.to_string());
        }
    }
    GIVEN("A Prefix Expression") {
        std::string input = "+ a b";
        Polynomial p(input);
        THEN("Polynomial is a+b") {
            REQUIRE("(+ a b)" == p.to_string());
        }
    }
}

SCENARIO("Testing Polynomial Interface Class", "[poly]") {
    GIVEN("The Expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
        std::string input = "(+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))";
        Parser parser;
        Expression e = parser.parse(input);

        Polynomial p = Polynomial(e);
        WHEN("Variables are not replaced") {
            const Env empty_env = {};
            THEN("Result should be (+ (* y 2) x 3)") {
                Polynomial result(p.evaluate(empty_env));
                REQUIRE(result.to_string() == "(+ (* y 2) x 3)");
            }
        }

        WHEN("x takes value 1") {
            const Env partial_env = {{"x", 1}};
            THEN("Result should be (+(* y 2) 4)") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE(result.to_string() == "(+ (* y 2) 4)");
                Expression const_expr = result.get_constant_part();
                AND_THEN("constant part of the function should be 4") {
                    NBR const *c = Factories::get_as_cst(const_expr.get());
                    REQUIRE(*c == 4);
                }
                AND_WHEN("constant part is subtracted") {
                    Polynomial sub(result.subtract_constant(const_expr));
                    THEN("subtraction should be (- (+ (* y 2) 4) 4)") {
                        REQUIRE(sub.to_string() == "(- (+ (* y 2) 4) 4)");
                    }
                }
            }
        }
    }
}
