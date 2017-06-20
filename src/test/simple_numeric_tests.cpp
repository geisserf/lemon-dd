#include "../catamorphism.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing basic numeric functions and evaluations") {
  GIVEN("The expression 10-5") {
    WHEN("empty environment") {
      std::cout << "\033[1;33m 10-5\033[0m" << '\n';
      expression e1 = sub({cst(10), cst(5)});
      THEN("Result should be 5") { REQUIRE(eval({}, e1) == 5); }
    }
  }
}
