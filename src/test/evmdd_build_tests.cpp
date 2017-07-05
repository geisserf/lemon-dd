#include "../Polynomial.h"
#include "../catamorph/EvmddExpression.h"
#include "../catamorph/interpreters/CreateEvmdd.h"
#include "Catch/include/catch.hpp"

SCENARIO("Testing basic EVMDD construction", "[evmddBuild]") {
    GIVEN("The expression (13) ") {
        std::string e = "13";
        Polynomial p = Polynomial(e);
        WHEN("No evaluation") {
            THEN("Result should be ") {
                Domains d = {};
                CreateEvmdd<EvmddExpression<float>> create;
                Evmdd<EvmddExpression<float>> res =
                    create.create_evmdd(p.getExpression(), d);
                REQUIRE(res.get_min().size() == 1);
                REQUIRE(res.get_min()[0].get()->value == 13);
            }
        }
    }
}
