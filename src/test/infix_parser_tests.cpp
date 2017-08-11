#include "../polynomial.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing parsing infix to prefix", "[infixparser]") {
    GIVEN("The expresison (a+b)") {
        std::string e = "(a+b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.toString() == "(+ a b)");
            }
        }
    }
    GIVEN("The expresison (a-b)") {
        std::string e = "(a-b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.toString() == "(- a b)");
            }
        }
    }
    GIVEN("The expresison (a*b)") {
        std::string e = "(a*b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.toString() == "(* a b)");
            }
        }
    }
    GIVEN("The expresison (a/b)") {
        std::string e = "(a/b)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.toString() == "(/ a b)");
            }
        }
    }

    GIVEN("The expresison ((a+b*c/(d+3))*[a==1])") {
        std::string e = "((((a+b)*c)/(d-13))*[a==1])";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                REQUIRE(p.toString() ==
                        "(* (/ (* (+ a b) c) (- d 13)) (== a 1))");
            }
        }
    }

    GIVEN("The Expression [!a]") {
        std::string e = "[!a]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "!(a)");
            }
        }
    }

    GIVEN("The Expression [a || b]") {
        std::string e = "[a || b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "(|| a b)");
            }
        }
    }

    GIVEN("The Expression [a && b]") {
        std::string e = "[a && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "(^ a b)");
            }
        }
    }
    GIVEN("The Expression [a == b]") {
        std::string e = "[a == b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "(== a b)");
            }
        }
    }

    GIVEN("The Expression [!a && b]") {
        std::string e = "[!a && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "(^ !(a) b)");
            }
        }
    }

    GIVEN("The Expression [(a+1) && b]") {
        std::string e = "[(a+1) && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "(^ (+ a 1) b)");
            }
        }
    }

    GIVEN("The Expression [ ![(a+1) || c] && b]") {
        std::string e = "[ ![(a+1) || c] && b]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "(^ !((|| (+ a 1) c)) b)");
            }
        }
    }

    GIVEN("The Expression [![a]]") {
        std::string e = "[![a]]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "!(a)");
            }
        }
    }

    GIVEN("The Expression (a+b+c)") {
        std::string e = "(a+b+c)";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be") {
                REQUIRE(p.toString() == "(+ a (+ b c))");
            }
        }
    }
}
