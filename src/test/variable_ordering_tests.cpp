#include "../catamorph/interpreters/dependencies.h"
#include "../catamorph/interpreters/variable_ordering.h"
#include "../parser.h"
#include "Catch/include/catch.hpp"

using std::string;

// Testing dependencies of expressions
SCENARIO("Testing Variable Ordering", "[expression]") {
    GIVEN("the term (1) with variables a and b") {
        std::vector<string> vars = {"a", "b"};
        std::string input = "1";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE(order[0] == "a");
            REQUIRE(order[1] == "b");
        }
    }
    GIVEN("the term (a)") {
        std::vector<string> vars = {"a"};
        std::string input = "a";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE(order[0] == "a");
        }
    }
    GIVEN("the term (a+b) * c") {
        std::vector<string> vars = {"a", "b", "c"};
        std::string input = "(a+b)*c";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE((order[0] == "a" || order[0] == "b"));
            REQUIRE((order[1] == "a" || order[1] == "b"));
            REQUIRE(order[2] == "c");
        }
    }
}
