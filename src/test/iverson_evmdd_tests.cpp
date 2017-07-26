#include "../catamorph/factories.h"
#include "../catamorph/interpreters/create_evmdd.h"
#include "../evmdd/evmdd_expression.h"
#include "../polynomial.h"
#include "Catch/include/catch.hpp"
#include <iostream>

SCENARIO("Testing iverson EVMDD construction", "[iversonevmdd]") {
    GIVEN("The expression (a & b)") {
        Expression e = Factories::land(
            {Factories::var((ID) "a"), Factories::var((ID) "b")});

        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);

            for (int a = 0; a <= 1; ++a) {
                for (int b = 0; b <= 1; ++b) {
                    std::vector<int> values_a{a};
                    std::vector<int> values_b{b};
                    std::map<std::string, std::vector<int>> state{
                        {"a", values_a}, {"b", values_b}};
                    auto result = evmdd.evaluate_partial(state);
                    THEN("result shoud be a & b") {
                        REQUIRE(result[0].value == (a && b));
                    }
                }
            }
        }
    }

    GIVEN("The expression (a & b) *10") {
        Expression e =
            Factories::mul({Factories::land({Factories::var((ID) "a"),
                                             Factories::var((ID) "b")}),
                            Factories::cst(10)});
        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);
            THEN("Evaluation with a = 1 & B=1 should be 10") {
                std::vector<int> values_a{1};
                std::vector<int> values_b{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a},
                                                              {"b", values_b}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 10);
            }
            THEN("get min should be 0") {
                std::map<std::string, std::vector<int>> state;
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 0);
            }
        }
    }

    GIVEN("The expression (a == b)") {
        Expression e = Factories::equals(
            {Factories::var((ID) "a"), Factories::var((ID) "b")});

        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);

            THEN("Evaluation with a = 1 & b=1 should be 1") {
                std::vector<int> values_a{1};
                std::vector<int> values_b{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a},
                                                              {"b", values_b}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 1);
            }
            THEN("get min should be 0") {
                std::map<std::string, std::vector<int>> state;
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 0);
            }
        }
    }

    GIVEN("The expression (a == b) * 10") {
        Expression e =
            Factories::mul({Factories::equals({Factories::var((ID) "a"),
                                               Factories::var((ID) "b")}),
                            Factories::cst(10)});

        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);

            THEN("Evaluation with a = 1 & b=1 should be 10") {
                std::vector<int> values_a{1};
                std::vector<int> values_b{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a},
                                                              {"b", values_b}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 10);
            }
            THEN("get min should be 0") {
                std::map<std::string, std::vector<int>> state;
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 0);
            }
        }
    }

    GIVEN("The expression (a || b)") {
        Expression e = Factories::lor(
            {Factories::var((ID) "a"), Factories::var((ID) "b")});

        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);

            THEN("Evaluation with a = 1 & b=1 should be 1") {
                std::vector<int> values_a{1};
                std::vector<int> values_b{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a},
                                                              {"b", values_b}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 1);
            }
            THEN("get min should be 0") {
                std::map<std::string, std::vector<int>> state;
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 0);
            }
        }
    }

    GIVEN("The expression (a || b) * 10") {
        Expression e =
            Factories::mul({Factories::lor({Factories::var((ID) "a"),
                                            Factories::var((ID) "b")}),
                            Factories::cst(10)});

        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);

            THEN("Evaluation with a = 1 & b=1 should be 10") {
                std::vector<int> values_a{1};
                std::vector<int> values_b{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a},
                                                              {"b", values_b}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 10);
            }
            THEN("Evaluation with a = 0 & b=1 should be 10") {
                std::vector<int> values_a{0};
                std::vector<int> values_b{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a},
                                                              {"b", values_b}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 10);
            }
            THEN("Evaluation with a = 1 & b=0 should be 10") {
                std::vector<int> values_a{1};
                std::vector<int> values_b{0};
                std::map<std::string, std::vector<int>> state{{"a", values_a},
                                                              {"b", values_b}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 10);
            }
            THEN("get min should be 0") {
                std::map<std::string, std::vector<int>> state;
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 0);
            }
        }
    }

    GIVEN("THE expression !(a)") {
        Expression e = Factories::lnot({Factories::var((ID) "a")});
        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}};
            Ordering o = {{"a", 1}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);
            THEN("Evaluation a=1 should be 0") {
                std::vector<int> values_a{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 0);
            }
            THEN("Evaluation a=0 should be 1") {
                std::vector<int> values_a{0};
                std::map<std::string, std::vector<int>> state{{"a", values_a}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 1);
            }
        }
    }
    GIVEN("THE expression !(a)*10") {
        Expression e = Factories::mul(
            {Factories::lnot({Factories::var((ID) "a")}), Factories::cst(10)});
        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}};
            Ordering o = {{"a", 1}};
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> evmdd = create.create_evmdd(e, d, o);
            // evmdd.print(std::cout);
            THEN("Evaluation a=1 should be 0") {
                std::vector<int> values_a{1};
                std::map<std::string, std::vector<int>> state{{"a", values_a}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 0);
            }
            THEN("Evaluation a=0 should be 10") {
                std::vector<int> values_a{0};
                std::map<std::string, std::vector<int>> state{{"a", values_a}};
                auto result = evmdd.evaluate_partial(state);
                REQUIRE(result[0].value == 10);
            }
        }
    }
}
