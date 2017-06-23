#include "Catch/include/catch.hpp"
#include "../catamorph/interpreters/Evaluate.h"



SCENARIO("Testing basic numeric functions and evaluations") {

  GIVEN("The Expression 10-5") {
    Expression e = Factories::sub({Factories::cst(10), Factories::cst(5)});
    WHEN("empty environment") {
      THEN("Result should be 5") {
        REQUIRE(Evaluate::eval({}, e) == 5);
      }
    }
  }
  GIVEN("The Expression 10+5") {
    Expression e = Factories::add({Factories::cst(10), Factories::cst(5)});
    WHEN("empty environment") {
      THEN("Result should be 15") {
        REQUIRE(Evaluate::eval({}, e) == 15);
      }
    }
  }
  GIVEN("The Expression 10/2") {
    Expression e = Factories::div({Factories::cst(10), Factories::cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 5") {
        REQUIRE(Evaluate::eval({}, e) == 5);
      }
    }
  }

  GIVEN("The Expression 10*2") {
    Expression e = Factories::mul({Factories::cst(10), Factories::cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 20") {
        REQUIRE(Evaluate::eval({}, e) == 20);
      }
    }
  }

  GIVEN("The Expression 9/2") {
    Expression e = Factories::div({Factories::cst(9), Factories::cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 4,5") {
        REQUIRE(Evaluate::eval({}, e) == 4.5);
      }
    }
  }

  GIVEN("The Expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
    Expression e = Factories::add({Factories::cst(1), Factories::cst(2), Factories::mul({Factories::cst(0), Factories::var((ID) "x"), Factories::var((ID) "y")}),
                                   Factories::mul({Factories::cst(1), Factories::var((ID) "y"), Factories::cst(2)}), Factories::add({Factories::cst(0), Factories::var((ID) "x")})});
    WHEN("Environment: x=1,y=2") {
      Env full_env = {{"x", 1}, {"y", 2}};
      THEN("Result should be 8.0") {
        REQUIRE(Evaluate::eval(full_env, e) == 8.0);
      }
    }
    WHEN("Environment: y=2") {
      Env partial_env = {{"y", 2}};
      THEN("Result should be 8.0") {
        auto o =Evaluate::partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }
  }


}
