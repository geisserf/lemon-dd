#include "../../../catamorph/interpreters/create_evmdd.h"
#include "../../../polynomial.h"
#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;
/********************************************************************
 *
 * Testing construction for expressions with logical propositions
 *
 ********************************************************************/
SCENARIO("Testing numeric EVMDDs for construction on iverson based input",
         "[evmdd][construction][iverson]") {
    GIVEN("Term [a == b] with domain 0,1,2") {
        Polynomial p = Polynomial("[a == b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 3}, {"b", 3}};
            Ordering o = {"b", "a"}; // {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 0 0 0" << endl;
                expected << "b 1 0 0" << endl;
                expected << "b 0 1 0" << endl;
                expected << "b 0 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term [![a&&b]] with domain 0,1") {
        Polynomial p = Polynomial("[![a&&b]]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {"b", "a"}; // {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 1 0" << endl;
                expected << "b 0 0" << endl;
                expected << "b 1 0" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term [!a] with domain 0,1") {
        Polynomial p = Polynomial("[!a]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}};
            Ordering o = {"a"}; // {{"a", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 1 0" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term [!!a] with domain 0,1") {
        Polynomial p = Polynomial("[!!a]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}};
            Ordering o = {"a"}; // {{"a", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    // If shannon reduction feature added, update accordingly.
    // (will have less nodes). Similar cases: [a||1]
    // @etavas
    GIVEN("Term [a && b] with domains Da = Db = {0,1}") {
        Polynomial p = Polynomial("[a&&b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {"b", "a"}; // {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 0 0" << endl;
                expected << "b 0 0" << endl;
                expected << "b 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term [a || b] with domains Da = Db = {0,1}") {
        Polynomial p = Polynomial("[a || b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {"b", "a"}; // {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 0 1" << endl;
                expected << "b 0 1" << endl;
                expected << "b 0 0" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    // If shannon reduction feature added,
    // update below accordingly. (will have less nodes)
    // @etavas
    GIVEN("Term [[!a]&&b] with domain 0,1") {
        Polynomial p = Polynomial("[[!a]&&b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {"b", "a"}; // {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 0 0" << endl;
                expected << "b 0 1" << endl;
                expected << "b 0 0" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }
    /***************************************************************
     *
     * Tests for combinations of arithmetic and iversion expressions
     *
     ****************************************************************/
    GIVEN("Term [!a]+b with domain 0,1") {
        Polynomial p = Polynomial("[!a]+b");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {"b", "a"}; // {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 1 0" << endl;
                expected << "b 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    GIVEN("Term [a&&[!b]]*c+5 with domain 0,1") {
        Polynomial p = Polynomial("[a&&[!b]]*c+5");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}, {"c", 2}};
            Ordering o = {"c","b","a"}; //{{"a", 3}, {"b", 2}, {"c", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 5" << endl;
                expected << "a 0 0" << endl;
                expected << "b 0 0" << endl;
                expected << "b 0 0" << endl;
                expected << "c 0 0" << endl; // [a&&[!b]] = 0
                expected << "c 0 1" << endl; // [a&&[!b]] = 1
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    /****************************************************************
     *
     *  Tests for logical expressions on non-bool. variable valuations
     *
     *****************************************************************/
    //  TODO : Edit source code to throw exceptions in this case
    //  IMPORTANT : == is valid for non-bool. values

    GIVEN("Term [a==b] with domain Da=Db={0,1,2}") {
        Polynomial p = Polynomial("[a==b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 3}, {"b", 3}};
            Ordering o = {"b","a"};// {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 0 0 0" << endl;
                expected << "b 1 0 0" << endl;
                expected << "b 0 1 0" << endl;
                expected << "b 0 0 1" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }

    // From this point, tests do not run correctly, because the source code
    // is not edited accordingly yet. @etavas

    GIVEN("Term [!a] with domain 0,1,2") {
        Polynomial p = Polynomial("[!a]");
        Domains d = {{"a", 3}};
        Ordering o = {"a"}; // {{"a", 1}};
        WHEN("We create the evmdd") {
            THEN("We get an error") {
                REQUIRE_THROWS_AS(p.create_evmdd<int>(d, o), std::logic_error);
            }
        }
    }
    GIVEN("Term [a && b] with domains Da ={0,1} Db = {0,1,2}") {
        Polynomial p = Polynomial("[a&&b]");
        Domains d = {{"a", 2}, {"b", 3}};
        Ordering o = {"b","a"}; // {{"a", 2}, {"b", 1}};
        WHEN("We create the evmdd") {
            THEN("We get an error") {
                REQUIRE_THROWS_AS(p.create_evmdd<int>(d, o), std::logic_error);
            }
        }
    }
    GIVEN("Term [a || b] with domains Da = Db = {0,1,2}") {
        Polynomial p = Polynomial("[a || b]");
        Domains d = {{"a", 3}, {"b", 3}};
        Ordering o = {"b","a"}; //{{"a", 2}, {"b", 1}};
        WHEN("We create the evmdd") {
            THEN("We get an error") {
                REQUIRE_THROWS_AS(p.create_evmdd<int>(d, o), std::logic_error);
            }
        }
    }
    // TODO : If shannon reduction is implemented, update below accordingly.
    // (will have less nodes)
    // (@etavas)
    GIVEN("Term [[!a]&&b] with domain 0,1,2") {
        Polynomial p = Polynomial("[[!a]&&b]");
        Domains d = {{"a", 3}, {"b", 3}};
        Ordering o = {"b", "a"}; //{{"a", 2}, {"b", 1}};
        WHEN("We create the evmdd") {
            THEN("We get an error") {
                REQUIRE_THROWS_AS(p.create_evmdd<int>(d, o), std::logic_error);
            }
        }
    }
    GIVEN("Term [!a]+b with domain Da=Db={0,1,2}") {
        Polynomial p = Polynomial("[!a]+b");
        Domains d = {{"a", 3}, {"b", 3}};
        Ordering o = {"b","a"}; //{{"a", 2}, {"b", 1}};
        WHEN("We create the evmdd") {
            THEN("We get an error") {
                REQUIRE_THROWS_AS(p.create_evmdd<int>(d, o), std::logic_error);
            }
        }
    }
    // Valid example
    GIVEN("Term [!a]+b with domain Da={0,1} Db={0,1,2}") {
        Polynomial p = Polynomial("[!a]+b");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 3}};
            Ordering o = {"b","a"};// {{"a", 2}, {"b", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("evmdd has the expected structure") {
                std::stringstream result;
                evmdd.print(result);
                std::stringstream expected;
                expected << "input: 0" << endl;
                expected << "a 1 0" << endl;
                expected << "b 0 1 2" << endl;
                REQUIRE(result.str() == expected.str());
            }
        }
    }
}