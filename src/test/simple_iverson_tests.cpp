#include "Catch/include/catch.hpp"
#include "../catamorph/interpreters/evaluate.h"

SCENARIO("Testing basic logic expressions and evaluations") {
  GIVEN("The expression x==2") {
    expression e = factories::equals({factories::var((id) "x"), factories::cst(2)});
    WHEN("empty environment") {
      env partial_env = {};
      THEN("Result should be 5") {
        auto o =evaluate::partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }

    WHEN("environment: x==2") {
      env partial_env = {{"x",2}};
      THEN("Result should be 5") {
        auto o =evaluate::partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }
  }
}