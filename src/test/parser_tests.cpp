#include "Catch/include/catch.hpp"
#include "../catamorph/Printer.h"
#include "../parser.h"

using std::string;
using std::vector;

SCENARIO("Testing the lexer on a simple expression", "[lexer]") {
    GIVEN("The expression (+ +12.5 -2 p_i_g)") {
        Lexer lexer("(+ +12.5 -2 p_i_g)");
        Token token = lexer.getNextToken();
        WHEN("We parse the first token") {
            THEN("We get a ( token") {
                REQUIRE(token.type == Token::LPAREN);
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the second token") {
            THEN("We get an operand token with value +") {
                REQUIRE(token.type == Token::OP);
                REQUIRE(token.value == "+");
            }
        }
        lexer.revert();
        token = lexer.getNextToken();
        WHEN("We revert the lexer") {
            THEN("We get again an operand token with value +") {
                REQUIRE(token.type == Token::OP);
                REQUIRE(token.value == "+");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the third token") {
            THEN("We get a constant token with value 12.5") {
                REQUIRE(token.type == Token::CONST);
                REQUIRE(token.value == "+12.5");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the fourth token") {
            THEN("We get a constant token with value -2") {
                REQUIRE(token.type == Token::CONST);
                REQUIRE(token.value == "-2");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the fifth token") {
            THEN("We get a variable token with value p_i_g") {
                REQUIRE(token.type == Token::VAR);
                REQUIRE(token.value == "p_i_g");
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the sixth token") {
            THEN("We get a ) token") {
                REQUIRE(token.type == Token::RPAREN);
            }
        }
        token = lexer.getNextToken();
        WHEN("We parse the last token") {
            THEN("We get a terminal token") {
                REQUIRE(token.type == Token::END);
            }
        }
    }
}

SCENARIO("Testing the lexer on an expression without whitespace", "[lexer]") {
    GIVEN("The expression (*(+12.5-2)(- 2pig))") {
        Lexer lexer("(*(++12.5-2)(- 2pig))");
        Token token = lexer.getNextToken();
        REQUIRE(token.type == Token::LPAREN);

        token = lexer.getNextToken();
        REQUIRE(Token::OP == token.type);
        REQUIRE("*" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::LPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Token::OP == token.type);
        REQUIRE("+" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::CONST == token.type);
        REQUIRE("+12.5" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::CONST == token.type);
        REQUIRE("-2" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Token::LPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Token::OP == token.type);
        REQUIRE("-" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::CONST == token.type);
        REQUIRE("2" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::VAR == token.type);
        REQUIRE("pig" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Token::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Token::END == token.type);
    }
}

SCENARIO("Testing the lexer on an expression with weird input", "[lexer]") {
    GIVEN("The expression (parant-) pig co3w") {
        Lexer lexer("(parant-) pig co3w");
        Token token = lexer.getNextToken();
        REQUIRE(Token::LPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Token::VAR == token.type);
        REQUIRE("parant" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::OP == token.type);
        REQUIRE("-" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::RPAREN == token.type);

        token = lexer.getNextToken();
        REQUIRE(Token::VAR == token.type);
        REQUIRE("pig" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::VAR == token.type);
        REQUIRE("co" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::CONST == token.type);
        REQUIRE("3" == token.value);

        token = lexer.getNextToken();
        REQUIRE(Token::VAR == token.type);
        REQUIRE("w" == token.value);
    }
}

SCENARIO("Testing the parser on arithmetic expressions", "[parser]") {
    GIVEN("The expression (+ 2 x)") {
        Parser parser;
        WHEN("We parse the expression") {
            Expression expr = parser.parse("(+ 2 x)");
            AND_WHEN("We print the expression") {
                string result = Printer::asString(expr);
                THEN("the result is (+ 2.000000 x)") {
                    REQUIRE(result == "(+ 2.000000 x)");
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
