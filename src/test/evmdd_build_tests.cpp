#include "../Polynomial.h"
#include "../catamorph/EvmddExpression.h"
#include "../catamorph/interpreters/CreateEvmdd.h"
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
            // TODO fix bug
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
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);
            THEN("get_min should be 0 and get_max should be 8") {
                REQUIRE(res.get_min()[0].value == 0);
                REQUIRE(res.get_max()[0].value == 8);
            }
        }
        WHEN("Partial Evaluation on x=0..4 y=0..2") {
            Domains d = {{"x", 5}, {"y", 3}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);

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
                        REQUIRE(res.evaluate_partial(state)[0].value == x * y);
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
                        REQUIRE(res.evaluate_partial(state)[0].value == x - y);
                    }
                }
            }
        }
    }
    GIVEN("the expression (x / y) with domain x=5, y=3") {
        std::string e = "x / y";
        Polynomial p = Polynomial(e);
        WHEN("Partial Evaluation on x=1..4 y=1..2") {
            Domains d = {{"x", 5}, {"y", 3}};
            Ordering o = {{"x", 1}, {"y", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d, o);

            for (float x = 0; x <= 4; x++) {
                for (float y = 1; y <= 2; y++) {
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
                        REQUIRE(res.evaluate_partial(state)[0].value == x / y);
                    }
                }
            }
        }
    }
}
