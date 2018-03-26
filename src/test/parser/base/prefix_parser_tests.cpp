#include "../../Catch/include/catch.hpp"
#include "../catamorph/printer.h"
#include "../parser.h"

#include <iostream>

using std::string;
using std::vector;

SCENARIO("Testing the prefixparser on valid arithmetic expressions",
         "[parser][prefixparser][valid]") {
    GIVEN("The expression (+ 2.5 x)") {
        Parser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse("(+ 2.5 x)");
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (+ 2.5 x)") {
                    REQUIRE(result == "(+ 2.5 x)");
                }
            }
        }
    }
    GIVEN("The expression (* a b)") {
        Parser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse("(* a b)");
            AND_WHEN("We print the expression") {
                string result = Printer::as_string(expr);
                THEN("the result is (* a b)") {
                    REQUIRE(result == "(* a b)");
                }
            }
        }
    }
}

SCENARIO("Testing the prefixparser on invalid expressions",
         "[parser][prefixparser][invalid]") {
    GIVEN("The expression '' ") {
        string s = "";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression ' ' ") {
        string s = " ";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression (") {
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse("("), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression )") {
        string s = ")";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression ()") {
        string s = "()";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression )(") {
        string s = ")(";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression /") {
        string s = "/";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression * +") {
        string s = "* +";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression (* +)") {
        string s = "(* +)";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression --)") {
        string s = "--)";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression 5 * 2") {
        string s = "5 * 2";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression 5 2 +") {
        string s = "5 2 +";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression * 2 +") {
        string s = "* 2 +";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression a / b)") {
        string s = "a / b)";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
    GIVEN("The expression / a b)") {
        string s = "/ a b)";
        Parser parser;
        WHEN("We parse the expression") {
            THEN("We get an invalid argument error") {
                REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
            }
        }
    }
}
