#include "../catamorph/factories.h"
#include "../catamorph/interpreters/create_evmdd.h"
#include "../polynomial.h"

#include "Catch/include/catch.hpp"
#include <iostream>

SCENARIO("Testing iverson EVMDD construction", "[iversonevmdd]") {
    GIVEN("The expression (a & b)") {
        Expression e = Factories::land(
            {Factories::var((ID) "a"), Factories::var((ID) "b")});

        Domains d = {{"a", 2}, {"b", 2}};
        Ordering o = {{"a", 1}, {"b", 2}};
        CreateEvmdd<float> create;
        auto evmdd = create.create_evmdd(e, d, o);

        ConcreteState state(2);
        WHEN("We evaluate the corresponding EVMDD for all domain values") {
            for (int a = 0; a <= 1; ++a) {
                for (int b = 0; b <= 1; ++b) {
                    state[0] = a;
                    state[1] = b;
                    THEN("The result should be a & b") {
                        REQUIRE(evmdd.evaluate(state) == (a && b));
                    }
                }
            }
        }
    }

    GIVEN("The expression (a & b) * 10") {
        Expression e =
            Factories::mul({Factories::land({Factories::var((ID) "a"),
                                             Factories::var((ID) "b")}),
                            Factories::cst(10)});
        WHEN("We create the EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<float> create;
            auto evmdd = create.create_evmdd(e, d, o);
            THEN("Evaluation with a = 1 & b = 1 should be 10") {
                ConcreteState state{1, 1};
                REQUIRE(evmdd.evaluate(state) == 10);
            }
            THEN("the minimum should be 0") {
                REQUIRE(evmdd.get_min() == 0);
            }
        }
    }

    GIVEN("The expression (a == b)") {
        Expression e = Factories::equals(
            {Factories::var((ID) "a"), Factories::var((ID) "b")});

        WHEN("We create the EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};

            CreateEvmdd<double> create;
            auto evmdd = create.create_evmdd(e, d, o);
            THEN("Evaluation with a = 1 & b = 1 should be 1") {
                ConcreteState state{1, 1};
                REQUIRE(evmdd.evaluate(state) == 1);
            }
            THEN("the minimum should be 0") {
                REQUIRE(evmdd.get_min() == 0);
            }
        }
    }

    GIVEN("The expression (a == b) * 10") {
        Expression e =
            Factories::mul({Factories::equals({Factories::var((ID) "a"),
                                               Factories::var((ID) "b")}),
                            Factories::cst(10)});
        WHEN("We create the EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<double> create;
            auto evmdd = create.create_evmdd(e, d, o);

            THEN("Evaluation with a = 1 & b = 1 should be 10") {
                ConcreteState state{1, 1};
                REQUIRE(evmdd.evaluate(state) == 10);
            }
            THEN("the minimum should be 0") {
                REQUIRE(evmdd.get_min() == 0);
            }
        }
    }

    GIVEN("The expression (a || b)") {
        Expression e = Factories::lor(
            {Factories::var((ID) "a"), Factories::var((ID) "b")});

        WHEN("We create the EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<int> create;
            auto evmdd = create.create_evmdd(e, d, o);

            THEN("Evaluation with a = 1 & b = 1 should be 1") {
                ConcreteState state{1, 1};
                REQUIRE(evmdd.evaluate(state) == 1);
            }
            THEN("the minimum should be 0") {
                REQUIRE(evmdd.get_min() == 0);
            }
        }
    }

    GIVEN("The expression (a || b) * 10") {
        Expression e =
            Factories::mul({Factories::lor({Factories::var((ID) "a"),
                                            Factories::var((ID) "b")}),
                            Factories::cst(10)});
        WHEN("We create the EVMDD") {
            Domains d = {{"a", 2}, {"b", 2}};
            Ordering o = {{"a", 1}, {"b", 2}};
            CreateEvmdd<int> create;
            auto evmdd = create.create_evmdd(e, d, o);

            THEN("Evaluation with a = 1 & b = 1 should be 10") {
                ConcreteState state{1, 1};
                REQUIRE(evmdd.evaluate(state) == 10);
            }
            THEN("Evaluation with a = 0 & b = 1 should be 10") {
                ConcreteState state{0, 1};
                REQUIRE(evmdd.evaluate(state) == 10);
            }
            THEN("Evaluation with a = 1 & b = 0 should be 10") {
                ConcreteState state{1, 0};
                REQUIRE(evmdd.evaluate(state) == 10);
            }
            THEN("the minimum should be 0") {
                REQUIRE(evmdd.get_min() == 0);
            }
        }
    }

    GIVEN("The expression !(a)") {
        Expression e = Factories::lnot({Factories::var((ID) "a")});
        WHEN("We create the EVMDD") {
            Domains d = {{"a", 2}};
            Ordering o = {{"a", 1}};
            CreateEvmdd<float> create;
            auto evmdd = create.create_evmdd(e, d, o);
            THEN("Evaluation a = 1 should be 0") {
                ConcreteState state{1};
                REQUIRE(evmdd.evaluate(state) == 0);
            }
            THEN("Evaluation a = 0 should be 1") {
                ConcreteState state{0};
                REQUIRE(evmdd.evaluate(state) == 1);
            }
        }
    }

    GIVEN("THE expression !(a)*10") {
        Expression e = Factories::mul(
            {Factories::lnot({Factories::var((ID) "a")}), Factories::cst(10)});
        WHEN("Create EVMDD") {
            Domains d = {{"a", 2}};
            Ordering o = {{"a", 1}};
            CreateEvmdd<float> create;
            auto evmdd = create.create_evmdd(e, d, o);
            THEN("Evaluation a = 1 should be 0") {
                ConcreteState state{1};
                REQUIRE(evmdd.evaluate(state) == 0);
            }
            THEN("Evaluation a = 0 should be 10") {
                ConcreteState state{0};
                REQUIRE(evmdd.evaluate(state) == 10);
            }
        }
    }
}
