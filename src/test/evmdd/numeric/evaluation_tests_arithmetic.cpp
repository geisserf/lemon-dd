#include "../../../polynomial.h"
#include "../../Catch/include/catch.hpp"

#include <cmath>
#include <iostream>

using std::endl;

// Testing evmdd::evaluate() for expressions without logical propositions
SCENARIO("Testing numeric EVMDDs for concrete evaluation over float",
         "[evmdd][evaluation][float]") {
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

    GIVEN("Evmdd for term abs(x-y) with domain x=0..3, y=0..3") {
        Polynomial p = Polynomial("abs(x-y)");
        Domains d = {{"x", 4}, {"y", 4}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<int>(d, o);
        THEN("Evaluation should be correct") {
            for (unsigned int x = 0; x < d["x"]; ++x) {
                for (unsigned int y = 0; y < d["y"]; ++y) {
                    ConcreteState state{x, y};
                    int sub = static_cast<int>(x) - y;
                    REQUIRE(evmdd.evaluate(state) == std::abs(sub));
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
            REQUIRE(evmdd.get_max() == 4 * 9 * 9 + 3 * 4 + 2);
        }
    }

    GIVEN("A large expression") {
        Polynomial p = Polynomial(
            "[var32==3]*9+[var32==0]*9+([var33==3]*4+[var33==0]*4)+([var34==3]*"
            "5+[var34==0]*5)+([var35==2]*8+[var35==0]*8)+([var36==2]*16+[var36="
            "=0]*16)+([var37==1]*10+[var37==0]*10)+([var38==1]*12+[var38==0]*"
            "12)+([var39==1]*10+[var39==0]*10)+([var40==1]*8+[var40==0]*8)+(["
            "var41==3]*14+[var41==0]*14)+([var42==3]*7+[var42==0]*7)+([var43=="
            "3]*9+[var43==0]*9)+([var44==3]*10+[var44==0]*10)+([var45==3]*7+["
            "var45==0]*7)+([var46==3]*11+[var46==0]*11)+([var47==1]*9+[var47=="
            "0]*9)+([var48==3]*6+[var48==0]*6)+([var49==1]*5+[var49==0]*5)+(["
            "var50==1]*9+[var50==0]*9)");
        Domains d = {{"var32", 4}, {"var33", 4}, {"var34", 4}, {"var35", 3},
                     {"var36", 3}, {"var37", 3}, {"var38", 3}, {"var39", 3},
                     {"var40", 3}, {"var41", 4}, {"var42", 4}, {"var43", 4},
                     {"var44", 4}, {"var45", 4}, {"var46", 4}, {"var47", 3},
                     {"var48", 4}, {"var49", 3}, {"var50", 3}};
        Ordering o = {
            {"var32", 1},  {"var33", 2},  {"var34", 3},  {"var35", 4},
            {"var36", 5},  {"var37", 6},  {"var38", 7},  {"var39", 8},
            {"var40", 9},  {"var41", 10}, {"var42", 11}, {"var43", 12},
            {"var44", 13}, {"var45", 14}, {"var46", 15}, {"var47", 16},
            {"var48", 17}, {"var49", 18}, {"var50", 19}};
        auto evmdd = p.create_evmdd<double>(d, o);
        THEN("get_min should be 0 and get_max should be 5") {
            REQUIRE(evmdd.get_min() == 0);
            REQUIRE(evmdd.get_max() == 169);
        }
    }
}

SCENARIO("Testing numeric EVMDDs for partial evaluation over float",
         "[numeric_evaluation]") {
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
