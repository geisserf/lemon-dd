#include "../../../catamorph/interpreters/create_evmdd.h"
#include "../../../polynomial.h"
#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;

SCENARIO("Testing numeric EVMDD construction", "[numeric_evmdd_construction]") {
    GIVEN("Term 5") {
        Polynomial p = Polynomial("5");
        WHEN("We create the evmdd") {
            Domains d = {};
            Ordering o = {};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("EVMDD only has input value and terminal node") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 5" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term x with domain 0,1,2") {
        Polynomial p = Polynomial("x");
        WHEN("We create the evmdd") {
            Domains d = {{"x", 3}};
            Ordering o = {{"x", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("EVMDD has one variable node with 5 edges") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 1 2" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term 5*x with domain 0,1,2") {
        Polynomial p = Polynomial("5*x");
        WHEN("We create the evmdd") {
            Domains d = {{"x", 3}};
            Ordering o = {{"x", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has one variable node with 5 edges") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 5 10" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term (5*x)*y with D(x)=0,1,2 and D(y)=0,1") {
        Domains d = {{"x", 3}, {"y", 2}};
        Polynomial p = Polynomial("(5*x)*y");
        WHEN("We create the evmdd and x has higher order than y") {
            Ordering o = {{"x", 1}, {"y", 2}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "y 0 0" << endl;
                expected << "x 0 0 0" << endl;
                expected << "x 0 5 10" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
        WHEN("We create the evmdd and x has lower order than y") {
            Ordering o = {{"x", 2}, {"y", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0 0" << endl;
                expected << "y 0 0" << endl;
                expected << "y 0 5" << endl;
                expected << "y 0 10" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term (x*y)+z with D(x)=D(y)=0,1 and D(z)=0,1,2") {
        Domains d = {{"x", 2}, {"y", 2}, {"z",3}};
        Polynomial p = Polynomial("(x*y)+z");
        WHEN("We create the evmdd in order z>x>y") {
            Ordering o = {{"x", 3}, {"y", 2}, {"z",1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0" << endl;
                expected << "y 0 0" << endl;
                expected << "y 0 1" << endl;
                expected << "z 0 1 2" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }
}
