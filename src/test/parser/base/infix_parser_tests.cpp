#include "../../Catch/include/catch.hpp"
#include "../catamorph/printer.h"
#include "../parser.h"

#include <iostream>

using std::string;
using std::vector;

SCENARIO("Testing the infixparser on valid arithmetic expressions",
         "[parser][infixparser][valid]") {
    GIVEN("The expression 2 + var1") {
        string s = "2 + var1";
        InfixParser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (+ 2 x)") {
                    REQUIRE(result == "(+ 2 var1)");
                }
            }
        }
    }

    GIVEN("The expression var +1") {
        string s = "var +1";
        InfixParser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (+ var 1)") {
                    REQUIRE(result == "(+ var 1)");
                }
            }
        }
    }
    GIVEN("The expression (a * b)") {
        string s = "(a * b)";
        InfixParser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (* a b)") {
                    REQUIRE(result == "(* a b)");
                }
            }
        }
    }
    GIVEN("The expression a / b") {
        string s = "a / b";
        InfixParser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (/ a b)") {
                    REQUIRE(result == "(/ a b)");
                }
            }
        }
    }
    GIVEN("The expression 2 + (5 * c)") {
        string s = "2 + (5 * c)";
        InfixParser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (+ 2 (* 5 c))") {
                    REQUIRE(result == "(+ 2 (* 5 c))");
                }
            }
        }
    }
    GIVEN("The expression (a * (b + c))") {
        string s = "(a * (b + c))";
        InfixParser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (* a (+ b c))") {
                    REQUIRE(result == "(* a (+ b c))");
                }
            }
        }
    }
    GIVEN("The expression 2 * + 5") {
        string s = "2 * + 5";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("Invalid Argument Exception is thrown") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }

    GIVEN("The expression (Var82*((Var0*((Var95/Var68)*((12 - Var21))))*32))") {
        string s = "(Var82*((Var0*((Var95/Var68)*((12 - Var21))))*32))";
        InfixParser parser;
        THEN("Parsing the expression throws no error") {
            REQUIRE_NOTHROW(parser.parse(s));
        }
    }
}

SCENARIO("Testing the infixparser on invalid expressions",
         "[parser][infixparser][invalid]") {
    GIVEN("The expression (") {
        string s = "(";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression )") {
        string s = ")";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression 2 + * 5") {
        string s = "2 + * 5";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }

    GIVEN("The expression 2 + * a") {
        string s = "2 + * a";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression 2 * + a 5") {
        string s = "2 * + a 5";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression a / b)") {
        string s = "a / b)";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression --)") {
        string s = "--)";
        InfixParser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
}
