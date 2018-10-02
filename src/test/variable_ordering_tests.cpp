#include "../catamorph/interpreters/dependencies.h"
#include "../catamorph/interpreters/variable_ordering.h"
#include "../parser.h"
#include "Catch/include/catch.hpp"

using std::string;

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
    GIVEN("the term c*(a+b)") {
        std::vector<string> vars = {"a", "b", "c"};
        std::string input = "c*(a+b)";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE((order[0] == "a" || order[0] == "b"));
            REQUIRE((order[1] == "a" || order[1] == "b"));
            REQUIRE(order[2] == "c");
        }
    }
    GIVEN("the term (a*b)+(2*(c*d))") {
        std::vector<string> vars = {"a", "b", "c", "d"};
        std::string input = "(a*b)+(2*(c*d))";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE((order[0] == "c" || order[0] == "d"));
            REQUIRE((order[1] == "c" || order[1] == "d"));
            REQUIRE((order[2] == "a" || order[2] == "b"));
            REQUIRE((order[3] == "a" || order[3] == "b"));
        }
    }
    GIVEN("an absolute amount term") {
        std::vector<string> vars = {"a"};
        std::string input = "abs(a)";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE(order[0] == "a");
        }
    }
    GIVEN("An iverson term") {
        std::vector<string> vars = {"a", "b", "c", "d"};
        std::string input = "[d||[![a&&b]]||!c]";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE((order[0] == "a" || order[0] == "b"));
            REQUIRE((order[1] == "a" || order[1] == "b"));
            REQUIRE((order[2] == "c"));
            REQUIRE((order[3] == "d"));
        }
    }
    GIVEN("An inequality term") {
        std::vector<string> vars = {"a", "b", "c", "d", "e", "f", "g", "h"};
        std::string input = "(a>b)+((c>=d)*(e<f)*(g<=h))";
        VariableOrdering ordering;
        auto order = ordering.get_fan_in_ordering(input, vars);
        THEN("Variable Ordering should be ...") {
            REQUIRE((order[0] == "g" || order[0] == "h"));
            REQUIRE((order[2] == "e" || order[2] == "f"));
            REQUIRE((order[4] == "c" || order[4] == "d"));
            REQUIRE((order[6] == "a" || order[6] == "b"));
        }
    }
}
