#include "../../../catamorph/interpreters/create_evmdd.h"
#include "../../../evmdd/abstract_factory.h"
#include "../../../polynomial.h"

#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;

// Testing construction for expressions without logical propositions
SCENARIO("Testing numeric EVMDD for construction on arithmetic input",
         "[evmdd][construction][arithmetic]") {
    GIVEN("Term 5") {
        Polynomial p = Polynomial("5");
        WHEN("We create the evmdd") {
            Domains d = {};
            Ordering o = {};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd only has input value and terminal node") {
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
            Ordering o = {"x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has one variable node with 3 edges") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 1 2" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term x with D(x)=0,1,2 and D(y)=0,1") {
        Polynomial p = Polynomial("x");
        WHEN("We create the evmdd") {
            Domains d = {{"x", 3}, {"y",2}};
            Ordering o = {"x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has one variable node with 3 edges") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 1 2" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has two nodes") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "y 0 0" << endl;
                expected << "x 0 1 2" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term 5*x with domain 0,1,2") {
        Polynomial p = Polynomial("5*x");
        WHEN("We create the evmdd") {
            Domains d = {{"x", 3}};
            Ordering o = {"x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has one variable node with 3 edges") {
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
        WHEN("We create the evmdd and x is under y") {
            Ordering o = {"x", "y"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "y 0 0" << endl;
                expected << "x 0 5 10" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
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
        WHEN("We create the evmdd and x is over y") {
            Ordering o = {"y", "x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0 0" << endl;
                expected << "y 0 5" << endl;
                expected << "y 0 10" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
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
        Domains d = {{"x", 2}, {"y", 2}, {"z", 3}};
        Polynomial p = Polynomial("(x*y)+z");
        WHEN("We create the evmdd in order x over y over z") {
            Ordering o = {"z", "y", "x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
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
        WHEN("We create the evmdd in order x over z over y") {
            Ordering o = {"y", "z", "x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 0" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }
}

SCENARIO(
    "Testing numeric EVMDDs for construction with partial ordering on "
    "arithmetic input",
    "[evmdd][construction][arithmetic][partial]") {
    GIVEN("Term (x*y)+z with D(x)=D(y)=0,1 and D(z)=0,1,2") {
        Domains d = {{"x", 2}, {"y", 2}, {"z", 3}};
        Polynomial p = Polynomial("(x*y)+z");
        WHEN("We create the evmdd in order x over y") {
            Ordering o = {"y", "x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "x 0 0" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "x 0 0" << endl;
                expected << "y 0 0" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
        WHEN("We create the evmdd in order y over x") {
            Ordering o = {"x", "y"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 0" << endl;
                expected << "x 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 0" << endl;
                expected << "x 0 0" << endl;
                expected << "x 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
        WHEN("We create the evmdd in order x over z") {
            Ordering o = {"z", "x"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                // z gets printed first, since nodes are printed from left
                // children to right children and the left x child of y is
                // reduced away
                expected << "input: 0" << endl;
                expected << "y 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "x 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "y 0 0" << endl;
                expected << "x 0 0" << endl;
                expected << "x 0 1" << endl;
                expected << "z 0 1 2" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
        WHEN("We create the evmdd in order z over x") {
            Ordering o = {"x", "z"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "y 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "z 0 1 2" << endl;
                expected << "x 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "y 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "z 0 1 2" << endl;
                expected << "x 0 0" << endl;
                expected << "x 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
        WHEN("We create the evmdd in order y over z") {
            Ordering o = {"z", "y"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                // z gets printed first, since nodes are printed from left
                // children to right children and the left x child of y is
                // reduced away
                expected << "input: 0" << endl;
                expected << "x 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
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
        WHEN("We create the evmdd in order z over y") {
            Ordering o = {"y", "z"};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("reduced evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
            THEN("quasi-reduced evmdd has the expected structure") {
                auto &factory = AbstractFactory<int>::get_factory(d, o);
                evmdd = factory.quasi_reduce(evmdd);
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "x 0 0" << endl;
                expected << "z 0 1 2" << endl;
                expected << "z 0 1 2" << endl;
                expected << "y 0 0" << endl;
                expected << "y 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }
}
