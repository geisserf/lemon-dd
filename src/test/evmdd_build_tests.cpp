#include "../catamorph/interpreters/create_evmdd.h"
#include "../evmdd/evmdd_expression.h"
#include "../polynomial.h"
#include "Catch/include/catch.hpp"
#include <iostream>

using std::endl;

SCENARIO("Testing basic numeric EVMDD construction", "[numericEVMDDBuild]") {
    GIVEN("The expression (13) ") {
        std::string e = "13";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                Domains d = {};
                Ordering o = {};
                CreateEvmdd<NumericExpression> create;
                Evmdd<NumericExpression> res =
                    create.create_evmdd(p.getExpression(), d, o);
                // std::cout << e << std::endl;
                // res.print(std::cout);
                REQUIRE(res.get_min().size() == 1);
                REQUIRE(res.get_min()[0].value == 13);
            }
        }
    }

    GIVEN("the Expression (X) with domain x=5") {
        std::string e = "x";
        Polynomial p = Polynomial(e);
        WHEN("Create EVMDD") {
            Domains d = {{"x", 5}};
            Ordering o = {{"x", 1}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            // std::cout << e << std::endl;
            // res.print(std::cout);
            //    std::cout << "created" << e << std::endl;
            //    res.print();
            //    std::cout << "Eval" << std::endl;
            THEN("get_min should be 0 and get_max should be 4") {
                REQUIRE(res.get_min()[0].value == 0);
                REQUIRE(res.get_max()[0].value == 4);
            }
        }
    }

    GIVEN("the Expression (X+1) with domain x=2") {
        std::string e = "x + 1";
        Polynomial p = Polynomial(e);
        WHEN("Create EVMDD") {
            Domains d = {{"x", 2}};
            Ordering o = {{"x", 1}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            THEN("evmdd has one variable node for x and one terminal node") {
                std::stringstream ss;
                res.print(ss);
                std::stringstream expected;
                expected << "input value: 1 nodes:" << endl;
                expected << "ID: 1(x)" << endl;
                expected << "  w[0]: 0" << endl << "    ID: 0( )" << endl;
                expected << "  w[1]: 1" << endl << "    ID: 0( )" << endl;
                REQUIRE(ss.str() == expected.str());
            }
        }
    }

    GIVEN("the Expression (x*y+2) with domain x=2, y=2") {
        std::string e = "(x*y) + 2";
        Polynomial p = Polynomial(e);
        WHEN("Create EVMDD") {
            Domains d = {{"x", 2}, {"y", 2}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            // node construction ids: [var(w[0], w[1])]
            // 0: terminal, 1: x(0,1), 2:y(0,1), 3:x(0,0), 4:y(0,0)
            THEN("evmdd has one variable node for y and two for x") {
                std::stringstream ss;
                res.print(ss);
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

    GIVEN("the Expression (2*x) with domain x=2") {
        std::string e = "2 * x";
        Polynomial p = Polynomial(e);
        WHEN("Create EVMDD") {
            Domains d = {{"x", 2}};
            Ordering o = {{"x", 1}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            THEN("evmdd has one variable node for x and one terminal node") {
                std::stringstream ss;
                res.print(ss);
                std::stringstream expected;
                expected << "input value: 0 nodes:" << endl;
                expected << "ID: 2(x)" << endl;
                expected << "  w[0]: 0" << endl << "    ID: 0( )" << endl;
                expected << "  w[1]: 2" << endl << "    ID: 0( )" << endl;
                REQUIRE(ss.str() == expected.str());
            }
        }
    }

    GIVEN("the Expression (X+1) with domain x=5") {
        std::string e = "x + 1";
        Polynomial p = Polynomial(e);
        WHEN("Create EVMDD") {
            Domains d = {{"x", 5}};
            Ordering o = {{"x", 1}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            THEN("get_min should be 1 and get_max should be 5") {
                REQUIRE(res.get_min()[0].value == 1);
                REQUIRE(res.get_max()[0].value == 5);
            }
        }
    }

    GIVEN("the expression (x+y) with domain x=5, y=2") {
        std::string e = "x + y";
        Polynomial p = Polynomial(e);
        WHEN("Create EVMDD") {
            Domains d = {{"x", 5}, {"y", 2}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            // std::cout << e << std::endl;
            // res.print(std::cout);
            // std::cout << "created" << e << std::endl;
            // res.print();
            // std::cout << "Eval" << std::endl;
            THEN("get_min should be 0 and get_max should be 5") {
                REQUIRE(res.get_min()[0].value == 0);
                REQUIRE(res.get_max()[0].value == 5);
            }
        }
    }

    GIVEN("the expression (x*y) with domain x=5, y=3") {
        std::string e = "x * y";
        Polynomial p = Polynomial(e);
        WHEN("Create EVMDD") {
            Domains d = {{"x", 5}, {"y", 3}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd =
                create.create_evmdd(p.getExpression(), d, o);
            THEN("get_min should be 0 and get_max should be 8") {
                REQUIRE(evmdd.get_min()[0].value == 0);
                REQUIRE(evmdd.get_max()[0].value == 8);
            }

            WHEN("We evaluate a single state") {
                for (int x = 0; x <= 4; x++) {
                    for (int y = 0; y <= 2; y++) {
                        std::vector<int> values_x{x};
                        std::vector<int> values_y{y};
                        std::map<std::string, std::vector<int>> state{
                            {"x", values_x}, {"y", values_y}};
                        auto result = evmdd.evaluate_partial(state);
                        THEN("evaluate partial has only one result") {
                            REQUIRE(result.size() == 1);
                        }
                        THEN("evaluate partial should be x*y") {
                            REQUIRE(result[0].value == x * y);
                        }
                    }
                }
            }
            WHEN("We evaluate partial state x={1,2,3}, y={2,3} ") {
                std::vector<int> values_x{1, 2, 3};
                std::vector<int> values_y{2};
                std::map<std::string, std::vector<int>> state{{"x", values_x},
                                                              {"y", values_y}};
                auto result = evmdd.evaluate_partial(state);
                THEN("evaluate partial has only one result") {
                    REQUIRE(result.size() == 1);
                }
                THEN("evaluate partial should be 1*2 = 2") {
                    REQUIRE(result[0].value == 2);
                }
            }
        }
    }
    GIVEN("the expression (x - y) with domain x=5, y=3") {
        std::string e = "x - y";
        Polynomial p = Polynomial(e);
        WHEN("Partial Evaluation on x=2..4 y=0..2") {
            Domains d = {{"x", 5}, {"y", 3}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);

            // std::cout << e << std::endl;
            // res.print(std::cout);
            // std::cout << "created" << e << std::endl;
            // res.print();
            // std::cout << "Eval" << std::endl;
            for (int x = 4; x <= 4; x++) {
                for (int y = 2; y <= 2; y++) {
                    std::vector<int> values_x;
                    values_x.push_back(x);
                    std::vector<int> values_y;
                    values_y.push_back(y);
                    std::map<std::string, std::vector<int>> state;
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "x", values_x));
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "y", values_y));
                    THEN("evaluate partial should be x-y") {
                        REQUIRE(res.evaluate_partial(state).size() == 1);
                        REQUIRE(res.evaluate_partial(state)[0].value == x - y);
                    }
                }
            }
        }
    }

    GIVEN("The Expression 4*x*x +3*y+2 with domain x=10, y=5") {
        std::string e = "(((4 * x) * x) + (3 * y)) + 2";
        Polynomial p = Polynomial(e);
        Domains d = {{"x", 10}, {"y", 5}};
        Ordering o = {{"x", 1}, {"y", 2}};
        CreateEvmdd<NumericExpression> create;
        Evmdd<NumericExpression> res =
            create.create_evmdd(p.getExpression(), d, o);
        WHEN("Evaluating") {
            for (float x = 0; x <= 10; x++) {
                for (float y = 0; y <= 5; y++) {
                    std::vector<int> values_x;
                    values_x.push_back((int)x);
                    std::vector<int> values_y;
                    values_y.push_back((int)y);
                    std::map<std::string, std::vector<int>> state;
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "x", values_x));
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "y", values_y));
                    THEN("evaluate partial should be 4x*x+3y+c") {
                        REQUIRE(res.evaluate_partial(state)[0].value ==
                                4 * x * x + 3 * y + 2);
                    }
                }
            }
        }
    }
}
