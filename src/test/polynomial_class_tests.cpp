#include "../Polynomial.h"
#include "../catamorph/interpreters/Evaluate.h"
#include "Catch/include/catch.hpp"
#include <exception>
#include <iostream>

SCENARIO("Testing Polynomial Interface Class", "[poly]") {
  GIVEN("The Expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
    Expression e = Factories::add(
        {Factories::cst(1), Factories::cst(2),
         Factories::mul({Factories::cst(0), Factories::var((ID) "x"),
                         Factories::var((ID) "y")}),
         Factories::mul(
             {Factories::cst(1), Factories::var((ID) "y"), Factories::cst(2)}),
         Factories::add({Factories::cst(0), Factories::var((ID) "x")})});

    Polynomial p = Polynomial(e);
    WHEN("Environment is empty") {
      const Env empty_env = {};
      THEN("Result should be (+ (* y 2) x 3)") {
        p.evaluate(empty_env);
        REQUIRE(p.toString() == "(+ (* y 2.000000) x 3.000000)");
      }
    }

    WHEN("Environment: x=1") {
      const Env partial_env = {{"x", 1}};
      THEN("Result should be (+(* y 2) 4)") {
        p.evaluate(partial_env);
        REQUIRE(p.toString() == "(+ (* y 2.000000) 4.000000)");
        NBR const_ = p.getAndSubtractConst();
        REQUIRE(const_ == 4);
        REQUIRE(p.toString() == "(- (+ (* y 2.000000) 4.000000) 4.000000)");
      }
    }
  }
}