#include "Catch/include/catch.hpp"
#include "../eval.h"

SCENARIO("Testing basic numeric functions and evaluations") {
  GIVEN("The expression 10-5") {
    expression e = sub({cst(10), cst(5)});
    WHEN("empty environment") {
      THEN("Result should be 5") {
        REQUIRE(eval({}, e) == 5);
      }
    }
  }
  GIVEN("The expression 10+5") {
    expression e = add({cst(10), cst(5)});
    WHEN("empty environment") {
      THEN("Result should be 15") {
        REQUIRE(eval({}, e) == 15);
      }
    }
  }
  GIVEN("The expression 10/2") {
    expression e = div({cst(10), cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 5") {
        REQUIRE(eval({}, e) == 5);
      }
    }
  }

  GIVEN("The expression 10*2") {
    expression e = mul({cst(10), cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 20") {
        REQUIRE(eval({}, e) == 20);
      }
    }
  }

  GIVEN("The expression 9/2") {
    expression e = div({cst(9), cst(2)});
    WHEN("empty environment") {
      THEN("Result should be 4,5") {
        REQUIRE(eval({}, e) == 4.5);
      }
    }
  }

  GIVEN("The expression (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x))") {
    expression e = add({cst(1), cst(2), mul({cst(0), var((id) "x"), var((id) "y")}),
                        mul({cst(1), var((id) "y"), cst(2)}), add({cst(0), var((id) "x")})});
    WHEN("Environment: x=1,y=2") {
      env full_env = {{"x", 1}, {"y", 2}};
      THEN("Result should be 8.0") {
        REQUIRE(eval(full_env, e) == 8.0);
      }
    }
    WHEN("Environment: y=2") {
      env partial_env = {{"y", 2}};
      THEN("Result should be 8.0") {
        auto o =partial_eval(partial_env, e);
        FAIL("missing compare");
      }
    }
  }


}
