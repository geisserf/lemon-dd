#include "Catch/include/catch.hpp"
#include "../catamorph/interpreters/Evaluate.h"
#include "../Polynomial.h"
#include <exception>
#include <iostream>

SCENARIO("Testing Polynomial Interface Class", "[poly]") {
  GIVEN("The Expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
    Expression e = Factories::add({Factories::cst(1), Factories::cst(2), Factories::mul({Factories::cst(0), Factories::var((ID) "x"), Factories::var((ID) "y")}),
                                   Factories::mul({Factories::cst(1), Factories::var((ID) "y"), Factories::cst(2)}), Factories::add({Factories::cst(0), Factories::var((ID) "x")})});

    Polynomial p = Polynomial(e);
    WHEN("Environment is empty") {
      const Env empty_env = {};
      THEN("Result should be same as input") {
        p.evaluate(empty_env);
      }
    }
  }
}