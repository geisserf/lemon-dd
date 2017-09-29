#include "../catamorph/interpreters/create_evmdd.h"
#include "../polynomial.h"
#include "Catch/include/catch.hpp"
#include <iostream>

using std::endl;

SCENARIO("Testing basic numeric EVMDD construction", "[numericEVMDDBuild]") {
    GIVEN("Constant term 13 ") {
        std::string e = "13";
        Polynomial p = Polynomial(e);
        WHEN("We create the evmdd") {
            Domains d = {};
            Ordering o = {};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("Size is 1") {
                REQUIRE(evmdd.size() == 1);
            }
            THEN("Evaluating any state should be 13") {
                ConcreteState state;
                REQUIRE(evmdd.evaluate(state) == 13);
            }
        }
    }

    GIVEN("Term x with domain x=5") {
        std::string e = "x";
        Polynomial p = Polynomial(e);
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 5}};
            Ordering o = {{"x", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("Size is 2") {
                REQUIRE(evmdd.size() == 2);
            }
            THEN("get_min should be 0 and get_max should be 4") {
                REQUIRE(evmdd.get_min().get_value() == 0);
                REQUIRE(evmdd.get_max().get_value() == 4);
            }
        }
    }

    GIVEN("Term x+1 with domain x=2") {
        std::string e = "x + 1";
        Polynomial p = Polynomial(e);
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 2}};
            Ordering o = {{"x", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("Size is 2") {
                REQUIRE(evmdd.size() == 2);
            }
            THEN("Evmdd has one variable node for x and one terminal node") {
                std::stringstream ss;
                evmdd.print(ss);
                std::stringstream expected;
                expected << "input value: 1 nodes:" << endl;
                expected << "ID: 1(x)" << endl;
                expected << "  w[0]: 0" << endl << "    ID: 0( )" << endl;
                expected << "  w[1]: 1" << endl << "    ID: 0( )" << endl;
                REQUIRE(ss.str() == expected.str());
            }
        }
    }

    GIVEN("Term x*y+2 with domain x=2, y=2") {
        std::string e = "(x*y) + 2";
        Polynomial p = Polynomial(e);
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 2}, {"y", 2}};
            Ordering o = {{"x", 1}, {"y", 2}};
            auto evmdd = p.create_evmdd<double>(d, o);
            THEN("Size is 4") {
                REQUIRE(evmdd.size() == 4);
            }
            // node construction ids: [var(w[0], w[1])]
            // 0: terminal, 1: x(0,1), 2:y(0,1), 3:x(0,0), 4:y(0,0)
            THEN("Evmdd has one variable node for y and two for x") {
                std::stringstream ss;
                evmdd.print(ss);
                std::stringstream expected;
                expected << "input value: 2 nodes:" << endl;
                expected << "ID: 4(y)" << endl;
                expected << "  w[0]: 0" << endl << "    ID: 3(x)" << endl;
                expected << "      w[0]: 0" << endl
                         << "        ID: 0( )" << endl;
                expected << "      w[1]: 0" << endl
                         << "        ID: 0( )" << endl;
                expected << "  w[1]: 0" << endl << "    ID: 1(x)" << endl;
                expected << "      w[0]: 0" << endl
                         << "        ID: 0( )" << endl;
                expected << "      w[1]: 1" << endl
                         << "        ID: 0( )" << endl;
                REQUIRE(ss.str() == expected.str());
            }
        }
    }

    GIVEN("Term 2*x with domain x=2") {
        std::string e = "2 * x";
        Polynomial p = Polynomial(e);
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 2}};
            Ordering o = {{"x", 1}};
            auto evmdd = p.create_evmdd<float>(d, o);
            THEN("Size is 2") {
                REQUIRE(evmdd.size() == 2);
            }
            THEN("Evmdd has one variable node for x and one terminal node") {
                std::stringstream ss;
                evmdd.print(ss);
                std::stringstream expected;
                expected << "input value: 0 nodes:" << endl;
                expected << "ID: 2(x)" << endl;
                expected << "  w[0]: 0" << endl << "    ID: 0( )" << endl;
                expected << "  w[1]: 2" << endl << "    ID: 0( )" << endl;
                REQUIRE(ss.str() == expected.str());
            }
        }
    }

    GIVEN("Term X+1 with domain x=5") {
        std::string e = "x + 1";
        Polynomial p = Polynomial(e);
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 5}};
            Ordering o = {{"x", 1}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("get_min should be 1 and get_max should be 5") {
                REQUIRE(evmdd.get_min() == 1);
                REQUIRE(evmdd.get_max() == 5);
            }
        }
    }

    GIVEN("Term x+y with domain x=5, y=2") {
        std::string e = "x + y";
        Polynomial p = Polynomial(e);
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 5}, {"y", 2}};
            Ordering o = {{"x", 1}, {"y", 2}};
            auto evmdd = p.create_evmdd<float>(d, o);
            THEN("Size is 3") {
                REQUIRE(evmdd.size() == 3);
            }
            THEN("get_min should be 0 and get_max should be 5") {
                REQUIRE(evmdd.get_min() == 0);
                REQUIRE(evmdd.get_max() == 5);
            }
        }
    }

    GIVEN("Term x*y with domain x=5, y=3") {
        std::string e = "x * y";
        Polynomial p = Polynomial(e);
        WHEN("We create the EVMDD") {
            Domains d = {{"x", 5}, {"y", 3}};
            Ordering o = {{"x", 1}, {"y", 2}};
            auto evmdd = p.create_evmdd<int>(d, o);
            THEN("get_min should be 0 and get_max should be 8") {
                REQUIRE(evmdd.get_min() == 0);
                REQUIRE(evmdd.get_max() == 8);
            }

            WHEN("We evaluate a single state") {
                ConcreteState state(2);
                for (size_t x = 0; x < d["x"]; ++x) {
                    for (size_t y = 0; y < d["y"]; ++y) {
                        state[0] = x;
                        state[1] = y;
                        int result = evmdd.evaluate(state);
                        THEN("evaluate should be x*y") {
                            REQUIRE(result == x * y);
                        }
                    }
                }
            }

            WHEN("We evaluate the max of partial state x={1,2,3}, y={1,2} ") {
                auto eval = [](std::vector<Monoid<int>> const &vec) {
                    return *std::max_element(vec.begin(), vec.end());
                };
                std::vector<int> values_x{1, 2, 3};
                std::vector<int> values_y{1, 2};
                std::map<std::string, std::vector<int>> state{{"x", values_x},
                                                              {"y", values_y}};
                Monoid<int> result =
                    evmdd.evaluate_partial<Monoid<int>>(state, eval);
                THEN("partial evaluation should be 3*2 = 6") {
                    REQUIRE(result.get_value() == 6);
                }
            }
        }
    }

    GIVEN("Term x - y with domain x=5, y=3") {
        std::string e = "x - y";
        Polynomial p = Polynomial(e);
        Domains d = {{"x", 5}, {"y", 3}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<int>(d, o);

        WHEN("Evaluation on x=0..4 y=0..2") {
            ConcreteState state(2);
            for (size_t x = 0; x < d["x"]; ++x) {
                for (size_t y = 0; y <= d["y"]; ++y) {
                    state[0] = x;
                    state[1] = y;

                    THEN("Evaluation should be x-y") {
                        REQUIRE(evmdd.evaluate(state) == x - y);
                    }
                }
            }
        }
    }

    GIVEN("Term '4*x*x+3*y+2' with domain x=10, y=5") {
        std::string e = "(((4 * x) * x) + (3 * y)) + 2";
        Polynomial p = Polynomial(e);
        Domains d = {{"x", 10}, {"y", 5}};
        Ordering o = {{"x", 1}, {"y", 2}};
        auto evmdd = p.create_evmdd<double>(d, o);
        WHEN("Evaluating") {
            ConcreteState state(2);
            for (size_t x = 0; x < d["x"]; ++x) {
                for (size_t y = 0; y < d["y"]; ++y) {
                    state[0] = x;
                    state[1] = y;
                    THEN("Evaluate should be 4x*x+3y+2") {
                        REQUIRE(evmdd.evaluate(state) == 4 * x * x + 3 * y + 2);
                    }
                }
            }
        }
    }
}
