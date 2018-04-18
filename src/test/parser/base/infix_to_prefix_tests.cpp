#include "../../Catch/include/catch.hpp"
#include "../polynomial.h"

SCENARIO("Testing parsing infix to prefix",
         "[parser][infixparser][infix_to_prefix]") {
    GIVEN("The expression (a+b)") {
        std::string e = "(a+b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.to_string() == "(+ a b)");
            }
        }
    }
    GIVEN("The expression (a-b)") {
        std::string e = "(a-b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.to_string() == "(- a b)");
            }
        }
    }
    GIVEN("The expression (a*b)") {
        std::string e = "(a*b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.to_string() == "(* a b)");
            }
        }
    }
    GIVEN("The expression (a/b)") {
        std::string e = "(a/b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.to_string() == "(/ a b)");
            }
        }
    }

    GIVEN("The expression ((a+b*c/(d+3))*[a==1])") {
        std::string e = "((((a+b)*c)/(d-13))*[a==1])";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.to_string() ==
                        "(* (/ (* (+ a b) c) (- d 13)) (== a 1))");
            }
        }
    }

    GIVEN("The expression [!a]") {
        std::string e = "[!a]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "!(a)");
            }
        }
    }

    GIVEN("The expression [a || b]") {
        std::string e = "[a || b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(|| a b)");
            }
        }
    }

    GIVEN("The expression [a && b]") {
        std::string e = "[a && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(^ a b)");
            }
        }
    }
    GIVEN("The expression [a == b]") {
        std::string e = "[a == b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(== a b)");
            }
        }
    }

    GIVEN("The expression [!a && b]") {
        std::string e = "[!a && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(^ !(a) b)");
            }
        }
    }

    GIVEN("The expression [(a+1) && b]") {
        std::string e = "[(a+1) && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(^ (+ a 1) b)");
            }
        }
    }

    GIVEN("The expression [ ![(a+1) || c] && b]") {
        std::string e = "[ ![(a+1) || c] && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(^ !((|| (+ a 1) c)) b)");
            }
        }
    }

    GIVEN("The expression [![a]]") {
        std::string e = "[![a]]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "!(a)");
            }
        }
    }

    GIVEN("The expression (a+b+c)") {
        std::string e = "(a+b+c)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(+ a (+ b c))");
            }
        }
    }

    GIVEN("The expression (a *-5)") {
        std::string e = "(a * -5)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.to_string() == "(* a (- 0 5))");
            }
        }
    }
}
