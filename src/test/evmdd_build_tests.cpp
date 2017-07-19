#include "../catamorph/interpreters/create_evmdd.h"
#include "../evmdd/evmdd_expression.h"
#include "../polynomial.h"
#include "Catch/include/catch.hpp"
#include <iostream>

SCENARIO("Testing basic EVMDD construction", "[evmddBuild]") {
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
                res.print(std::cout);
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
            res.print(std::cout);
            //    std::cout << "created" << e << std::endl;
            //    res.print();
            //    std::cout << "Eval" << std::endl;
            THEN("get_min should be 0 and get_max should be 4") {
                REQUIRE(res.get_min()[0].value == 0);
                REQUIRE(res.get_max()[0].value == 4);
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
            res.print(std::cout);
            //  std::cout << "created" << e << std::endl;
            //  res.print();
            //  std::cout << "Eval" << std::endl;
            THEN("get_min should be 1 and get_max should be 5") {
                // REQUIRE(res.get_min()[0].value == 1);
                // REQUIRE(res.get_max()[0].value == 5);
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
            res.print(std::cout);
            // std::cout << "created" << e << std::endl;
            // res.print();
            // std::cout << "Eval" << std::endl;
            THEN("get_min should be 0 and get_max should be 5") {
                // REQUIRE(res.get_min()[0].value == 0);
                // REQUIRE(res.get_max()[0].value == 5);
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
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            res.print(std::cout);
            // std::cout << "created" << e << std::endl;
            // res.print();
            // std::cout << "Eval" << std::endl;
            THEN("get_min should be 0 and get_max should be 8") {
                // REQUIRE(res.get_min()[0].value == 0);
                // REQUIRE(res.get_max()[0].value == 8);
            }
        }
        WHEN("Partial Evaluation on x=0..4 y=0..2") {
            Domains d = {{"x", 5}, {"y", 3}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);

            res.print(std::cout);
            // std::cout << "created" << e << std::endl;
            // res.print();
            // std::cout << "Eval" << std::endl;
            for (int x = 0; x <= 4; x++) {
                for (int y = 0; y <= 2; y++) {
                    std::vector<int> values_x;
                    values_x.push_back(x);
                    std::vector<int> values_y;
                    values_y.push_back(y);
                    std::map<std::string, std::vector<int>> state;
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "x", values_x));
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "y", values_y));
                    THEN("evaluate partial should be x*y") {
                        // REQUIRE(res.evaluate_partial(state)[0].value == x *
                        // y);
                    }
                }
            }
        }
    }
    GIVEN("the expression (x - y) with domain x=5, y=3") {
        std::string e = "x - y";
        Polynomial p = Polynomial(e);
        WHEN("Partial Evaluation on x=1..4 y=1..2") {
            Domains d = {{"x", 5}, {"y", 3}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);

            res.print(std::cout);
            // std::cout << "created" << e << std::endl;
            // res.print();
            // std::cout << "Eval" << std::endl;
            for (float x = 2; x <= 4; x++) {
                for (float y = 0; y <= 2; y++) {
                    std::vector<int> values_x;
                    values_x.push_back((int)x);
                    std::vector<int> values_y;
                    values_y.push_back((int)y);
                    std::map<std::string, std::vector<int>> state;
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "x", values_x));
                    state.insert(std::pair<std::string, std::vector<int>>(
                        "y", values_y));
                    THEN("evaluate partial should be x*y") {
                        // REQUIRE(res.evaluate_partial(state)[0].value == x -
                        // y);
                    }
                }
            }
        }
    }

    //   GIVEN("the expression (x / y) with domain x=5, y=3") {
    //       std::string e = "x / y";
    //       Polynomial p = Polynomial(e);
    //       WHEN("Partial Evaluation on x=1..4 y=1..2") {
    //           Domains d = {{"x", 5}, {"y", 3}};
    //           Ordering o = {{"x", 1}, {"y", 2}};
    //           CreateEvmdd<NumericExpression> create;
    //           Evmdd<NumericExpression> res =
    //               create.create_evmdd(p.getExpression(), d, o);
    //
    //           for (float x = 0; x <= 4; x++) {
    //               for (float y = 1; y <= 2; y++) {
    //                   std::vector<int> values_x;
    //                   values_x.push_back((int)x);
    //                   std::vector<int> values_y;
    //                   values_y.push_back((int)y);
    //                   std::map<std::string, std::vector<int>> state;
    //                   state.insert(std::pair<std::string, std::vector<int>>(
    //                       "x", values_x));
    //                   state.insert(std::pair<std::string, std::vector<int>>(
    //                       "y", values_y));
    //                   THEN("evaluate partial should be x/y") {
    //                       REQUIRE(res.evaluate_partial(state)[0].value == x /
    //                       y);
    //                   }
    //               }
    //           }
    //       }
    //   }
    //

    GIVEN("The Expression 4*x*x +3*y+2 with domain x=10, y=5") {
        std::string e = "(((4 * x) * x) + (3 * y)) + 2";
        Polynomial p = Polynomial(e);
        Domains d = {{"x", 10}, {"y", 5}};
        Ordering o = {{"x", 1}, {"y", 2}};
        CreateEvmdd<NumericExpression> create;
        Evmdd<NumericExpression> res =
            create.create_evmdd(p.getExpression(), d, o);
        res.print(std::cout);
        // std::cout << "created" << e << std::endl;
        // res.print();
        // std::cout << "Eval" << std::endl;
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
                        // REQUIRE(res.evaluate_partial(state)[0].value ==
                        // 4 * x * x + 3 * y + 2);
                    }
                }
            }
        }
    }

    //    GIVEN("The Expression 6*u*u*u*u*u*u + 5*v*v*v*v*v + 4*w*w*w*w -
    //    3*x*x*x - 2*y*y -z + 50 domain x=10, y=5") {
    //        std::string e = "(((6*u)*))";
    //        Polynomial p = Polynomial(e);
    //       Domains d = {{"x", 10}, {"y", 5}};
    //        Ordering o = {{"x", 1}, {"y", 2}};
    //        CreateEvmdd<NumericExpression> create;
    //        Evmdd<NumericExpression> res =
    //            create.create_evmdd(p.getExpression(), d, o);
    //        WHEN("Evaluating") {
    //            for (float x = 0; x <= 10; x++) {
    //                for (float y = 0; y <=5; y++) {
    //                    std::vector<int> values_x;
    //                    values_x.push_back((int)x);
    //                    std::vector<int> values_y;
    //                    values_y.push_back((int)y);
    //                    std::map<std::string, std::vector<int>> state;
    //                    state.insert(std::pair<std::string, std::vector<int>>(
    //                       "x", values_x));
    //                    state.insert(std::pair<std::string, std::vector<int>>(
    //                        "y", values_y));
    //                    THEN("evaluate partial should be 4x*x+3y+c") {
    //                        REQUIRE(res.evaluate_partial(state)[0].value ==
    //                        4*x*x+3*y+2);
    //                    }
    //                }
    //            }
    //        }
    //    }
}
