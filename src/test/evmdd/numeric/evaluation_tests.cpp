#include "../../../polynomial.h"
#include "../../Catch/include/catch.hpp"
#include <iostream>

using std::endl;

SCENARIO("Testing concrete evaluation for numeric EVMDDs over float",
         "[numeric_evmdd_evaluation]") {
    GIVEN("Evmdd for term 13 ") {
        Polynomial p = Polynomial("13");
        auto evmdd = p.create_evmdd<double>({}, {});
        THEN("Evaluating any state should be 13") {
            ConcreteState state;
            REQUIRE(evmdd.evaluate(state) == 13);
        }
        THEN("get_min and get_max should be 13") {
            REQUIRE(evmdd.get_min() == 13);
            REQUIRE(evmdd.get_max() == 13);
        }
    }

    GIVEN("Evmdd for term x with domain 0..4") {
        Polynomial p = Polynomial("x");
        Domains d = {{"x", 5}};
        Ordering o = {{"x", 1}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluating state x:=1 should be 1") {
            ConcreteState state{1};
            REQUIRE(evmdd.evaluate(state) == 1);
        }
        THEN("get_min should be 0 and get_max should be 4") {
            REQUIRE(evmdd.get_min() == 0);
            REQUIRE(evmdd.get_max() == 4);
        }
    }

    GIVEN("Evmdd for term x+1 with domain 0,1,2") {
        Polynomial p = Polynomial("x+1");
        Domains d = {{"x", 3}};
        Ordering o = {{"x", 1}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluating state x:=2 should be 3") {
            ConcreteState state{2};
            REQUIRE(evmdd.evaluate(state) == 3);
        }
        THEN("get_min should be 1 and get_max should be 3") {
            REQUIRE(evmdd.get_min() == 1);
            REQUIRE(evmdd.get_max() == 3);
        }
    }

    GIVEN("Evmdd for term x*y+2 with domain x=0,1,2 and y=0,1") {
        Polynomial p = Polynomial("(x*y) + 2");
        Domains d = {{"x", 3}, {"y", 2}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<double>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int x = 0; x < d["x"]; ++x) {
                for (unsigned int y = 0; y < d["y"]; ++y) {
                    ConcreteState state{x, y};
                    REQUIRE(evmdd.evaluate(state) == x * y + 2);
                }
            }
        }

        THEN("get_min should be 2 and get_max should be 4") {
            REQUIRE(evmdd.get_min() == 2);
            REQUIRE(evmdd.get_max() == 4);
        }
    }

    GIVEN("Evmdd for term x+y with domain x=0..4, y=0,1") {
        Polynomial p = Polynomial("x+y");
        Domains d = {{"x", 5}, {"y", 2}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int x = 0; x < d["x"]; ++x) {
                for (unsigned int y = 0; y < d["y"]; ++y) {
                    ConcreteState state{x, y};
                    REQUIRE(evmdd.evaluate(state) == x + y);
                }
            }
        }
        THEN("get_min should be 0 and get_max should be 5") {
            REQUIRE(evmdd.get_min() == 0);
            REQUIRE(evmdd.get_max() == 5);
        }
    }

    GIVEN("Evmdd for term x - y with domain x=0..4, y=0,1,2") {
        Polynomial p = Polynomial("x-y");
        Domains d = {{"x", 5}, {"y", 3}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);

        THEN("Evaluation should be correct") {
            for (unsigned int x = 0; x < d["x"]; ++x) {
                for (unsigned int y = 0; y < d["y"]; ++y) {
                    ConcreteState state{x, y};
                    int result = x - y;
                    REQUIRE(evmdd.evaluate(state) == result);
                }
            }
        }
        THEN("get_min should be 0 and get_max should be 5") {
            REQUIRE(evmdd.get_min() == -2);
            REQUIRE(evmdd.get_max() == 4);
        }
    }

    GIVEN("Evmdd for term x*0.3 + 0.7  with domain x=0..4") {
        Polynomial p = Polynomial("x*0.3 + 0.7");
        Domains d = {{"x", 5}};
        Ordering o = {{"x", 1}};
        auto evmdd = p.create_evmdd<float>(d, o);

        THEN("Evaluation should be correct") {
            for (unsigned int x = 0; x < d["x"]; ++x) {
                for (unsigned int y = 0; y < d["y"]; ++y) {
                    ConcreteState state{x, y};
                    REQUIRE(evmdd.evaluate(state) == x * 0.3 + 0.7);
                }
            }
        }
    }

    GIVEN("Evmdd for term 4*x*x+3*y+2 with domain x=0..9, y=0..4") {
        Polynomial p = Polynomial("(((4 * x) * x) + (3 * y)) + 2");
        Domains d = {{"x", 10}, {"y", 5}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<double>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int x = 0; x < d["x"]; ++x) {
                for (unsigned int y = 0; y < d["y"]; ++y) {
                    ConcreteState state{x, y};
                    REQUIRE(evmdd.evaluate(state) ==
                            (((4 * x) * x) + (3 * y)) + 2);
                }
            }
        }
        THEN("get_min should be 0 and get_max should be 5") {
            REQUIRE(evmdd.get_min() == 2);
            REQUIRE(evmdd.get_max() == 4*9*9+3*4+2);
        }
    }
}

SCENARIO("Testing partial evaluation for numeric EVMDDs over float",
         "[numeric_evmdd_evaluation]") {
    GIVEN("Evmdd for term x*y with domain x=0..4, y=0,1,2") {
        Polynomial p = Polynomial("x*y");
        Domains d = {{"x", 5}, {"y", 3}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<float>(d, o);

        WHEN("We evaluate the max of partial state x={1,2,3}, y={1,2} ") {
            auto eval = [](std::vector<Monoid<float>> const &vec) {
                return *std::max_element(vec.begin(), vec.end());
            };

            std::vector<int> x_values{1, 2, 3};
            std::vector<int> y_values{1, 2};
            std::map<std::string, std::vector<int>> state{{"x", x_values},
                                                          {"y", y_values}};
            Monoid<float> result =
                evmdd.evaluate_partial<Monoid<float>>(state, eval);
            THEN("partial evaluation should be 3*2 = 6") {
                REQUIRE(result.get_value() == 6);
            }
        }
    }
}
