#include "../polynomial.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing parsing infix to prefix", "[infixparser]") {
    GIVEN("The expresison (a+b*c/(d+3))*[a==1]") {
        std::string e = "(a+b*c/(d-13))*[a==1]";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                p.print();
            }
        }
    }
}
