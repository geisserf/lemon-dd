#include "../catamorphism.h"
#include "Catch/include/catch.hpp"
#include "../factories.h"
#include "../eval.h"
#include <iostream>


SCENARIO("Testing basic numeric functions and evaluations") {
  GIVEN("The expression x==2") {
    expression e = equals({var((id) "x"), cst(2)});
    WHEN("empty environment") {
      env partial_env = {};
      THEN("Result should be 5") {
        auto o =partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }

    WHEN("environment: x=2") {
      env partial_env = {{"x",2}};
      THEN("Result should be 5") {
        auto o =partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }
  }
}