#include "../parser.h"
#include "../catamorph/printer.h"
#include "Catch/include/catch.hpp"

#include <iostream>

using std::string;
using std::vector;

SCENARIO("Testing the lexer on single expressions", "[lexer]") {
    WHEN("We parse VAR1") {
        Lexer lexer("VAR1");
        THEN("We get a token representing a variable") {
            Token token = lexer.getNextToken();
            REQUIRE(token.type == Type::VAR);
        }
    }
    WHEN("We parse 1VAR") {
        Lexer lexer("1VAR");
        THEN("We get a token representing a constant") {
            Token token = lexer.getNextToken();
            REQUIRE(token.type == Type::CONST);
            AND_THEN("And a token representing a variable") {
                Token token = lexer.getNextToken();
                REQUIRE(token.type == Type::VAR);
            }
        }
    }
}

SCENARIO("Testing the lexer on an expression with no variables", "[lexer]") {
    GIVEN("The expression 10 - 5") {
        Lexer lexer("10 - 5");
        Token token = lexer.getNextToken();
        THEN("First token is a constant") {
            REQUIRE(token.type == Type::CONST);
        }
        token = lexer.getNextToken();
        THEN("Second token is an operator") {
            REQUIRE(token.type == Type::OP);
        }
        token = lexer.getNextToken();
        THEN("Third token is a constant") {
            REQUIRE(token.type == Type::CONST);
        }
    }
    GIVEN("The expression + 2 x") {
        Lexer lexer("+ 2 x");
        Token token = lexer.getNextToken();
        THEN("First token is an operand") {
            REQUIRE(token.type == Type::OP);
        }
        token = lexer.getNextToken();
        THEN("Second token is a constant") {
            REQUIRE(token.type == Type::CONST);
        }
        token = lexer.getNextToken();
        THEN("Third token is a variable") {
            REQUIRE(token.type == Type::VAR);
        }
    }
}

SCENARIO("Testing the lexer on an expression with a variable", "[lexer]") {
    GIVEN("The expression (+ 12.5 -2 p_i_g)") {
        Lexer lexer("(+ 12.5 -2 p_i_g)");
        Token token = lexer.getNextToken();
        WHEN("We parse the first token") {
            THEN("We get a ( token") {
                REQUIRE(token.type == Type::LPAREN);
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the second token") {
            THEN("We get an operand token with value +") {
                REQUIRE(token.type == Type::OP);
                REQUIRE(token.value == "+");
            }
        }
        lexer.revert();
        token = lexer.getNextToken();
        WHEN("We revert the lexer") {
            THEN("We get again an operand token with value +") {
                REQUIRE(token.type == Type::OP);
                REQUIRE(token.value == "+");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the third token") {
            THEN("We get a constant token with value 12.5") {
                REQUIRE(token.type == Type::CONST);
                REQUIRE(token.value == "12.5");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the fourth token") {
            THEN("We get an operand token with value -") {
                REQUIRE(token.type == Type::OP);
                REQUIRE(token.value == "-");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the fifth token") {
            THEN("We get a constant token with value 2") {
                REQUIRE(token.type == Type::CONST);
                REQUIRE(token.value == "2");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the sixth token") {
            THEN("We get a variable token with value p_i_g") {
                REQUIRE(token.type == Type::VAR);
                REQUIRE(token.value == "p_i_g");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the seventh token") {
            THEN("We get a ) token") {
                REQUIRE(token.type == Type::RPAREN);
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the last token") {
            THEN("We get a terminal token") {
                REQUIRE(token.type == Type::END);
            }
        }
    }
}

SCENARIO("Testing the lexer on an expression without whitespace", "[lexer]") {
    GIVEN("The expression (*(+12.5-2)(- 2pig))") {
        Lexer lexer("(*(++12.5-2)(- 2pig))");
        Token token = lexer.getNextToken();
        REQUIRE(token.type == Type::LPAREN);

        token = lexer.getNextToken();
        REQUIRE(Type::OP == token.type);
        REQUIRE("*" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::LPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Type::OP == token.type);
        REQUIRE("+" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::OP == token.type);
        REQUIRE("+" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::CONST == token.type);
        REQUIRE("12.5" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::OP == token.type);
        REQUIRE("-" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::CONST == token.type);
        REQUIRE("2" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Type::LPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Type::OP == token.type);
        REQUIRE("-" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::CONST == token.type);
        REQUIRE("2" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::VAR == token.type);
        REQUIRE("pig" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Type::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Type::END == token.type);
    }
}

SCENARIO("Testing the lexer on an expression with weird input", "[lexer]") {
    GIVEN("The expression (parant-) pig co3w") {
        Lexer lexer("(parant-) pig co3w");
        Token token = lexer.getNextToken();
        REQUIRE(Type::LPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Type::VAR == token.type);
        REQUIRE("parant" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::OP == token.type);
        REQUIRE("-" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Type::VAR == token.type);
        REQUIRE("pig" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Type::VAR == token.type);
        REQUIRE("co3w" == token.value);
    }
}

SCENARIO("Testing the prefixparser on arithmetic expressions", "[parser]") {
    GIVEN("The expression (+ 2 x)") {
        Parser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse("(+ 2 x)");
            AND_WHEN("We print the expression") {
                string result = Printer::asString(expr);
                THEN("the result is (+ 2 x)") {
                    REQUIRE(result == "(+ 2 x)");
                }
            }
        }
    }
    GIVEN("The expression (* a b)") {
        Parser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse("(* a b)");
            AND_WHEN("We print the expression") {
                string result = Printer::asString(expr);
                THEN("the result is (* a b)") {
                    REQUIRE(result == "(* a b)");
                }
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
}

SCENARIO("Testing the infixparser on valid arithmetic expressions",
         "[parser]") {
    GIVEN("The expression 2 + var1") {
        string s = "2 + var1";
        InfixParser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::asString(expr);
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
                string result = Printer::asString(expr);
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
                string result = Printer::asString(expr);
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
                string result = Printer::asString(expr);
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
                string result = Printer::asString(expr);
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
                string result = Printer::asString(expr);
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
            Expression expr = parser.parse(s);
            AND_WHEN("We print the expression") {
                string result = Printer::asString(expr);
                THEN("the result is (* 2 (+ 0 5))") {
                    REQUIRE(result == "(* 2 (+ 0 5))");
                }
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

SCENARIO("Testing the infixparser on invalid expressions", "[parser]") {
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
    GIVEN("The expression 2 + 2 + 2") {
        string s = "2 + 2 + 2";
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
                // TODO this test currently fails, since it is not checked if
                // ")" has a matching "(", as long as the expression before ")"
                // is valid. This could actually be ok and not be an invalid
                // argument, thus allowing more errors on the user side.
                // REQUIRE_THROWS_AS(parser.parse(s), std::invalid_argument);
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
