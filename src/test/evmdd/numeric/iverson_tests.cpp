#include "../../../polynomial.h"
#include "../../Catch/include/catch.hpp"

#include <iostream>

SCENARIO("Testing concrete evaluation for iverson based EVMDDs ",
         "[iverson_evmdd_evaluation]") {
    GIVEN("Evmdd for expression [a && b])") {
        Polynomial p = Polynomial("[a&&b]");
        Domains d = {{"a", 2}, {"b", 2}};
        Ordering o = {{"a", 1}, {"b", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int a = 0; a < d["a"]; ++a) {
                for (unsigned int b = 0; b < d["b"]; ++b) {
                    ConcreteState state{a, b};
                    REQUIRE(evmdd.evaluate(state) == (a && b));
                }
            }
        }
    }

    GIVEN("Evmdd for expression [a && b] * 10") {
        Polynomial p = Polynomial("[a && b] * 10");
        Domains d = {{"a", 2}, {"b", 2}};
        Ordering o = {{"a", 1}, {"b", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int a = 0; a < d["a"]; ++a) {
                for (unsigned int b = 0; b < d["b"]; ++b) {
                    ConcreteState state{a, b};
                    REQUIRE(evmdd.evaluate(state) == (a && b) * 10);
                }
            }
        }
    }

    GIVEN("Evmdd for expression [a == b]") {
        Polynomial p = Polynomial("[a == b]");
        Domains d = {{"a", 2}, {"b", 2}};
        Ordering o = {{"a", 1}, {"b", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int a = 0; a < d["a"]; ++a) {
                for (unsigned int b = 0; b < d["b"]; ++b) {
                    ConcreteState state{a, b};
                    REQUIRE(evmdd.evaluate(state) == (a == b));
                }
            }
        }
    }

    GIVEN("Evmdd for expression [a == b] * 10") {
        Polynomial p = Polynomial("[a == b] * 10");
        Domains d = {{"a", 2}, {"b", 2}};
        Ordering o = {{"a", 1}, {"b", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int a = 0; a < d["a"]; ++a) {
                for (unsigned int b = 0; b < d["b"]; ++b) {
                    ConcreteState state{a, b};
                    REQUIRE(evmdd.evaluate(state) == (a == b) * 10);
                }
            }
        }
    }

    GIVEN("Evmdd for expression [a || b]") {
        Polynomial p = Polynomial("[a || b]");
        Domains d = {{"a", 2}, {"b", 2}};
        Ordering o = {{"a", 1}, {"b", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int a = 0; a < d["a"]; ++a) {
                for (unsigned int b = 0; b < d["b"]; ++b) {
                    ConcreteState state{a, b};
                    REQUIRE(evmdd.evaluate(state) == (a || b));
                }
            }
        }
    }

    GIVEN("Evmdd for expression [a || b] * 10") {
        Polynomial p = Polynomial("[a || b] * 10");
        Domains d = {{"a", 2}, {"b", 2}};
        Ordering o = {{"a", 1}, {"b", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int a = 0; a < d["a"]; ++a) {
                for (unsigned int b = 0; b < d["b"]; ++b) {
                    ConcreteState state{a, b};
                    REQUIRE(evmdd.evaluate(state) == (a || b) * 10);
                }
            }
        }
    }

    GIVEN("Evmdd for expression [!a]") {
        Polynomial p = Polynomial("[!a]");
        Domains d = {{"a", 2}};
        Ordering o = {{"a", 1}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluating a:= 1 should be 0") {
            ConcreteState state{1};
            REQUIRE(evmdd.evaluate(state) == 0);
        }
        THEN("Evaluating a:= 0 should be 1") {
            ConcreteState state{0};
            REQUIRE(evmdd.evaluate(state) == 1);
        }
    }

    GIVEN("Evmdd for expression [!a] * 10") {
        Polynomial p = Polynomial("[!a] * 10");
        Domains d = {{"a", 2}};
        Ordering o = {{"a", 1}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluating a:= 1 should be 0") {
            ConcreteState state{1};
            REQUIRE(evmdd.evaluate(state) == 0);
        }
        THEN("Evaluating a:= 0 should be 10") {
            ConcreteState state{0};
            REQUIRE(evmdd.evaluate(state) == 10);
        }
    }
}
