#include "Catch/include/catch.hpp"
#include "../catamorph/interpreters/evaluate.h"



SCENARIO("Testing basic numeric functions and evaluations") {

  GIVEN("The expression 10-5") {
    expression e = factories::sub({factories::cst(10), factories::cst(5)});
    WHEN("empty environment") {
      THEN("Result should be 5") {
        REQUIRE(evaluate::eval({}, e) == 5);
      }
    }
  }
  GIVEN("The expression 10+5") {
    expression e = factories::add({factories::cst(10), factories::cst(5)});
    WHEN("empty environment") {
      THEN("Result should be 15") {
        REQUIRE(evaluate::eval({}, e) == 15);
      }
    }
  }
  GIVEN("The expression 10/2") {
    expression e = factories::div({factories::cst(10), factories::cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 5") {
        REQUIRE(evaluate::eval({}, e) == 5);
      }
    }
  }

  GIVEN("The expression 10*2") {
    expression e = factories::mul({factories::cst(10), factories::cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 20") {
        REQUIRE(evaluate::eval({}, e) == 20);
      }
    }
  }

  GIVEN("The expression 9/2") {
    expression e = factories::div({factories::cst(9), factories::cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 4,5") {
        REQUIRE(evaluate::eval({}, e) == 4.5);
      }
    }
  }

  GIVEN("The expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
    expression e = factories::add({factories::cst(1), factories::cst(2), factories::mul({factories::cst(0), factories::var((id) "x"), factories::var((id) "y")}),
                                   factories::mul({factories::cst(1), factories::var((id) "y"), factories::cst(2)}), factories::add({factories::cst(0), factories::var((id) "x")})});
    WHEN("Environment: x=1,y=2") {
      env full_env = {{"x", 1}, {"y", 2}};
      THEN("Result should be 8.0") {
        REQUIRE(evaluate::eval(full_env, e) == 8.0);
      }
    }
    WHEN("Environment: y=2") {
      env partial_env = {{"y", 2}};
      THEN("Result should be 8.0") {
        auto o =evaluate::partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }
  }


}
