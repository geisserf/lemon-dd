#include "../catamorph/interpreters/Evaluate.h"
#include "../Polynomial.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing basic logic expressions and evaluations", "[iverson]") {
  GIVEN("The Expression x==2") {
    Expression e =
        Factories::equals({Factories::var((ID) "x"), Factories::cst(2)});
    Polynomial p = Polynomial(e);
    WHEN("empty environment") {
      Env partial_env = {};
      THEN("Result should be 5") {
        p.evaluate(partial_env);
        REQUIRE(p.toString()=="(== x 2.000000)");
      }
    }

    WHEN("environment: x==2") {
      Env partial_env = {{"x", 2}};
      Polynomial p = Polynomial(e);
      THEN("Result should be 1") {
        p.evaluate(partial_env);
        REQUIRE(p.toString()=="1.000000");
      }
    }
  }
}
