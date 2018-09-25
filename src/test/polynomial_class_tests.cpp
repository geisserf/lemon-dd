#include "../polynomial.h"
#include "../catamorph/interpreters/evaluate.h"
#include "../parser.h"
#include "Catch/include/catch.hpp"

#include <exception>
#include <iostream>
#include <string>

// Testing Polynomial interface class functions
SCENARIO("Testing Polynomial Interface Class", "[polynomial]") {
    // Testing: Class constructor
    // TEST: Polynomial for input as prefix and infix string
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
    // Testing : get_constant_part() and subtract_constant()
    // TEST : Expression with no constant
    GIVEN("The expression (* x 5)") {
        std::string input = "(* x 5)";
        Parser parser;
        Expression e = parser.parse(input);
        Polynomial p = Polynomial(e);
        WHEN("Variables are not replaced") {
            const Env empty_env = {};
            THEN("Constant part of the function should be 0") {
                Polynomial result(p.evaluate(empty_env));
                Expression const_expr = result.get_constant_part();
                NBR const *c = Factories::get_as_cst(const_expr.get());
                REQUIRE(*c == 0);
                // CASE : Subtraction when there is no constant
                AND_WHEN("constant part is subtracted") {
                    Polynomial sub(result.subtract_constant(const_expr));
                    THEN("subtraction should be (- (* x 5) 0)") {
                        REQUIRE(sub.to_string() == "(- (* x 5) 0)");
                    }
                }
            }
        }
        WHEN("x takes value 1") {
            const Env partial_env = {{"x", 1}};
            THEN("constant part of the function should be 5") {
                Polynomial result(p.evaluate(partial_env));
                Expression const_expr = result.get_constant_part();
                NBR const *c = Factories::get_as_cst(const_expr.get());
                REQUIRE(*c == 5);
                // CASE: There is a constant to subtract, after valuating
                // variable.
                AND_WHEN("constant part is subtracted") {
                    Polynomial sub(result.subtract_constant(const_expr));
                    THEN("subtraction should be (- 5 5)") {
                        REQUIRE(sub.to_string() == "(- 5 5)");
                    }
                }
            }
        }
    }
    // TEST : Expression with constant and variable
    GIVEN("The expression (+ x 3)") {
        std::string input = "(+ x 3)";
        Parser parser;
        Expression e = parser.parse(input);
        Polynomial p = Polynomial(e);
        WHEN("Variables are not replaced") {
            const Env empty_env = {};
            THEN("Constant part of the function should be 3") {
                Polynomial result(p.evaluate(empty_env));
                Expression const_expr = result.get_constant_part();
                NBR const *c = Factories::get_as_cst(const_expr.get());
                REQUIRE(*c == 3);
                // CASE : Subtract constant originally existing in input
                AND_WHEN("constant part is subtracted") {
                    Polynomial sub(result.subtract_constant(const_expr));
                    THEN("subtraction should be (- (+ x 3) 3)") {
                        REQUIRE(sub.to_string() == "(- (+ x 3) 3)"); // TODO
                    }
                }
            }
        }
        WHEN("x takes value 1") {
            const Env partial_env = {{"x", 1}};
            THEN("constant part of the function should be 4") {
                Polynomial result(p.evaluate(partial_env));
                Expression const_expr = result.get_constant_part();
                NBR const *c = Factories::get_as_cst(const_expr.get());
                REQUIRE(*c == 4);
                // CASE : Subtract constant when const.sum changes after
                // valuating a variable
                AND_WHEN("constant part is subtracted") {
                    Polynomial sub(result.subtract_constant(const_expr));
                    THEN("subtraction should be (- 4 4)") {
                        REQUIRE(sub.to_string() == "(- 4 4)");
                    }
                }
            }
        }
        WHEN("x takes value 0") {
            const Env partial_env = {{"x", 0}};
            THEN("constant part of the function should remain 3") {
                Polynomial result(p.evaluate(partial_env));
                Expression const_expr = result.get_constant_part();
                NBR const *c = Factories::get_as_cst(const_expr.get());
                REQUIRE(*c == 3);
                // CASE : Subtract constant when const.sum does not change after
                // valuating a variable
                AND_WHEN("constant part is subtracted") {
                    Polynomial sub(result.subtract_constant(const_expr));
                    THEN("subtraction should be (- 3 3)") {
                        REQUIRE(sub.to_string() == "(- 3 3)");
                    }
                }
            }
        }
    }
    // TESTING : substituting variables
    GIVEN("An Infix Expression") {
        std::string input = "(a-b)";
        Polynomial p(input);
        THEN("Polynomial is a-b") {
            REQUIRE("(- a b)" == p.to_string());
        }

        WHEN("a takes value 0") {
            const Env partial_env = {{"a", 0}};
            THEN("function should be 0 - b") {
                Polynomial result(p.evaluate(partial_env));
                REQUIRE("(- 0 b)" == result.to_string());
            }
        }
    }
    // TESTING : overall evaluation
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
            THEN("Result should be (+ (* y 2) 4)") {
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
// Simple iversion tests
SCENARIO("Testing Polynomial for evaluation with basic logic expressions",
         "[polynomial]") {
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
// Simple numeric tests
SCENARIO("Testing Polynomial for evaluation with basic numeric functions",
         "[polynomial]") {
    // GIVEN("The Expression 10-5") {
    //     Polynomial p("10 - 5");
    //     WHEN("empty environment") {
    //         Polynomial result(p.evaluate({}));
    //         THEN("Result should be 5") {
    //             REQUIRE(result.to_string() == "5");
    //         }
    //     }
    // }

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
