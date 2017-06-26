#include "Catch/include/catch.hpp"
#include "../catamorph/interpreters/Evaluate.h"

SCENARIO("Testing basic logic expressions and evaluations", "[iverson]") {
  GIVEN("The Expression x==2") {
    Expression e = Factories::equals({Factories::var((ID) "x"), Factories::cst(2)});
    WHEN("empty environment") {
      Env partial_env = {};
      THEN("Result should be 5") {
        auto o =Evaluate::partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }

    WHEN("environment: x==2") {
      Env partial_env = {{"x",2}};
      THEN("Result should be 5") {
        auto o =Evaluate::partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }
  }
}