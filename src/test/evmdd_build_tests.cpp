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
                CreateEvmdd<NumericExpression> create;
                Evmdd<NumericExpression> res =
                    create.create_evmdd(p.getExpression(), d);
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
            CreateEvmdd<NumericExpression> create;
            Evmdd<NumericExpression> res =
                create.create_evmdd(p.getExpression(), d);
            THEN("get_min should be 0 and get_max should be 4") {
                REQUIRE(res.get_min()[0].value == 0);
                REQUIRE(res.get_max()[0].value == 4);
            }
        }
    }
}
