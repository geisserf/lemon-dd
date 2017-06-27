#include "../catamorph/interpreters/Evaluate.h"
#include "../Polynomial.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing basic numeric functions and evaluations", "[numeric]") {
    GIVEN("The Expression 10-5") {
        Expression e = Factories::sub({Factories::cst(10), Factories::cst(5)});
        Polynomial p = Polynomial(e);
        WHEN("empty environment") {
            p.evaluate({});
            THEN("Result should be 5") {
                REQUIRE(p.toString() == "5.000000");
            }
        }
    }
    GIVEN("The Expression 10+5") {
        Expression e = Factories::add({Factories::cst(10), Factories::cst(5)});
        Polynomial p = Polynomial(e);
        WHEN("empty environment") {
            p.evaluate({});
            THEN("Result should be 15") {
                REQUIRE(p.toString() == "15.000000");
            }
        }
    }
    GIVEN("The Expression 10/2") {
        Expression e = Factories::div({Factories::cst(10), Factories::cst(2)});
        Polynomial p = Polynomial(e);
        WHEN("empty environment") {
            p.evaluate({});
            THEN("Result should be 5") {
                REQUIRE(p.toString() == "5.000000");
            }
        }
    }

    GIVEN("The Expression 10*2") {
        Expression e = Factories::mul({Factories::cst(10), Factories::cst(2)});
        Polynomial p = Polynomial(e);
        WHEN("empty environment") {
            p.evaluate({});
            THEN("Result should be 20") {
                REQUIRE(p.toString() == "20.000000");
            }
        }
    }

    GIVEN("The Expression 9/2") {
        Expression e = Factories::div({Factories::cst(9), Factories::cst(2)});
        Polynomial p = Polynomial(e);
        WHEN("empty environment") {
            p.evaluate({});
            THEN("Result should be 4,5") {
                REQUIRE(p.toString() == "4.500000");
            }
        }
    }

    GIVEN("The Expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
        Expression e = Factories::add(
            {Factories::cst(1), Factories::cst(2),
             Factories::mul({Factories::cst(0), Factories::var((ID) "x"),
                             Factories::var((ID) "y")}),
             Factories::mul({Factories::cst(1), Factories::var((ID) "y"),
                             Factories::cst(2)}),
             Factories::add({Factories::cst(0), Factories::var((ID) "x")})});
        Polynomial p = Polynomial(e);
        WHEN("Environment: x=1,y=2") {
            Env full_env = {{"x", 1}, {"y", 2}};
            THEN("Result should be 8.0") {
                p.evaluate(full_env);
                REQUIRE(p.toString() == "8.000000");
            }
        }
        WHEN("Environment: y=2") {
            Env partial_env = {{"y", 2}};
            THEN("Result should be (+ x 7.000000)") {
                p.evaluate(partial_env);
                REQUIRE(p.toString() == "(+ x 7.000000)");
            }
        }
    }
    GIVEN("The Expression (+ 1 2 (* 0 VAR1 VAR2) (* 1 VAR2 2) (+ 0 VAR1))") {
        Expression e = Factories::add(
            {Factories::cst(1), Factories::cst(2),
             Factories::mul({Factories::cst(0), Factories::var((ID) "VAR1"),
                             Factories::var((ID) "VAR2")}),
             Factories::mul({Factories::cst(1), Factories::var((ID) "VAR2"),
                             Factories::cst(2)}),
             Factories::add({Factories::cst(0), Factories::var((ID) "VAR1")})});
        Polynomial p = Polynomial(e);
        WHEN("Environment: VAR1=1,VAR2=2") {
            Env full_env = {{"VAR1", 1}, {"VAR2", 2}};
            THEN("Result should be 8.0") {
                p.evaluate(full_env);
                REQUIRE(p.toString() == "8.000000");
            }
        }
        WHEN("Environment: VAR2=2") {
            Env partial_env = {{"VAR2", 2}};
            THEN("Result should be (+ VAR1 7.000000)") {
                p.evaluate(partial_env);
                REQUIRE(p.toString() == "(+ VAR1 7.000000)");
            }
        }
    }
}
