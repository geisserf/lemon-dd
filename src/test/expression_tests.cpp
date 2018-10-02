#include "../parser.h"
#include "../catamorph/interpreters/dependencies.h"
#include "Catch/include/catch.hpp"

using std::string;

// Testing dependencies of expressions
SCENARIO("Testing Expression dependencies", "[expression]") {
    GIVEN("An addition over a,b,c") {
        std::string input = "(a+b)+c";
        InfixParser parser;
        Expression e = parser.parse(input);
        THEN("Expression depends on variables a,b,c") {
            std::set<string> dependencies = Dependency::dependencies(e);
            REQUIRE(dependencies.find("a") != dependencies.end());
            REQUIRE(dependencies.find("b") != dependencies.end());
            REQUIRE(dependencies.find("c") != dependencies.end());
        }
    }

    GIVEN("A subtraction over a,b,c") {
        std::string input = "(a-b)-c";
        InfixParser parser;
        Expression e = parser.parse(input);
        THEN("Expression depends on variables a,b,c") {
            std::set<string> dependencies = Dependency::dependencies(e);
            REQUIRE(dependencies.find("a") != dependencies.end());
            REQUIRE(dependencies.find("b") != dependencies.end());
            REQUIRE(dependencies.find("c") != dependencies.end());
        }
    }

    GIVEN("A multiplication over a,b,c") {
        std::string input = "(a*b)*c";
        InfixParser parser;
        Expression e = parser.parse(input);
        THEN("Expression depends on variables a,b,c") {
            std::set<string> dependencies = Dependency::dependencies(e);
            REQUIRE(dependencies.find("a") != dependencies.end());
            REQUIRE(dependencies.find("b") != dependencies.end());
            REQUIRE(dependencies.find("c") != dependencies.end());
        }
    }

    GIVEN("A division over a,b,c") {
        std::string input = "(a/b)/c";
        InfixParser parser;
        Expression e = parser.parse(input);
        THEN("Expression depends on variables a,b,c") {
            std::set<string> dependencies = Dependency::dependencies(e);
            REQUIRE(dependencies.find("a") != dependencies.end());
            REQUIRE(dependencies.find("b") != dependencies.end());
            REQUIRE(dependencies.find("c") != dependencies.end());
        }
    }

    GIVEN("An iverson expression over a,b,c,d") {
        std::string input = "[a==2&&b==3||c==5||!d]";
        InfixParser parser;
        Expression e = parser.parse(input);
        THEN("Expression depends on variables a,b,c,d") {
            std::set<string> dependencies = Dependency::dependencies(e);
            REQUIRE(dependencies.find("a") != dependencies.end());
            REQUIRE(dependencies.find("b") != dependencies.end());
            REQUIRE(dependencies.find("c") != dependencies.end());
            REQUIRE(dependencies.find("d") != dependencies.end());
        }
    }

    GIVEN("An arithmetic and iverson mixed expression over a,b,c,d") {
        std::string input = "[a==2]*[b&&c]+[!d]";
        InfixParser parser;
        Expression e = parser.parse(input);
        THEN("Expression depends on variables a,b,c,d") {
            std::set<string> dependencies = Dependency::dependencies(e);
            REQUIRE(dependencies.find("a") != dependencies.end());
            REQUIRE(dependencies.find("b") != dependencies.end());
            REQUIRE(dependencies.find("c") != dependencies.end());
            REQUIRE(dependencies.find("d") != dependencies.end());
        }
    }
}
