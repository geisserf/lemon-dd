#include "../../../catamorph/interpreters/create_evmdd.h"
#include "../../../polynomial.h"
#include "../../Catch/include/catch.hpp"

#include <iostream>

using std::endl;

// Testing construction for expressions with logical propositions
SCENARIO("Testing numeric EVMDDs for construction on iverson based input",
         "[evmdd][construction][iverson]") {
    GIVEN("Term [a == b] with domain 0,1,2") {
        Polynomial p = Polynomial("[a == b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 3}, {"b", 3}};
            Ordering o = {{"a", 2}, {"b", 1}};
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
            Ordering o = {{"a", 2}, {"b", 1}};
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

    /***********************************************************
     *
     * TODO : extend NOT tests for domain size > 2
     *        This is currently allowed by the program,
     *        but it should give an error.
     *
     *        @author Ece Tavaslı
     *
     ************************************************************/
    GIVEN("Term [!a] with domain 0,1") {
        Polynomial p = Polynomial("[!a]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}};
            Ordering o = {{"a", 1}};
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
            Ordering o = {{"a", 1}};
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

    /*********************************************************
     *
     * TODO : If you improve graphs for min. edges and nodes,
     *        update this accordingly.
     *        For example: a=0 ---> directly go to termination
     *                              do not make another b node.
     *        In that case, there won't be 2 nodes for b.
     *        Similar cases: [a||1]
     *
     * TODO : Update or implement seperately && and || for
     *        domain size 3
     *        Should give an error since bool. operations
     *        for domain size >2 is not allowed.
     *
     *        @author Ece Tavaslı
     *
     * *******************************************************/
    GIVEN("Term [a && b] with domains Da = Db = {0,1}") {
        Polynomial p = Polynomial("[a&&b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 2}, {"b", 1}};
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
            Ordering o = {{"a", 2}, {"b", 1}};
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

    /* ****************************************************
     * TODO : If #node and early termination feature added,
     *        update below accordingly. (will have less nodes)
     *
     * TODO: Extend below for domain size >2
     *       Should give an error since bool. operations
     *       for domain >2 is not allowed by the library.
     *
     *        @author Ece Tavaslı
     * *****************************************************/
    GIVEN("Term [[!a]&&b] with domain 0,1") {
        Polynomial p = Polynomial("[[!a]&&b]");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 2}, {"b", 1}};
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

    // Tests for combinations of arithmetic and iversion expressions
    GIVEN("Term [!a]+b with domain 0,1") {
        Polynomial p = Polynomial("[!a]+b");
        WHEN("We create the evmdd") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 2}, {"b", 1}};
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
            Ordering o = {{"a", 3}, {"b", 2}, {"c", 1}};
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
}