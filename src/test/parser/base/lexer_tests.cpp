#include "../../Catch/include/catch.hpp"
#include "../catamorph/printer.h"
#include "../parser.h"

#include <cmath>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;

SCENARIO("Testing the lexer on empty expression", "[lexer]") {
    WHEN("We parse empty string") {
        Lexer lexer("");
        THEN("We get a token representing the end") {
            Token token = lexer.getNextToken();
            REQUIRE(token.type == Type::END);
        }
    }
    WHEN("We parse space") {
        Lexer lexer(" ");
        THEN("We get a token representing the end") {
            Token token = lexer.getNextToken();
            REQUIRE(token.type == Type::END);
        }
    }
}

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
    WHEN("We parse abs") {
        Lexer lexer("abs(");
        THEN("We get a token representing an operand") {
            Token token = lexer.getNextToken();
            REQUIRE(token.type == Type::OP);
            token = lexer.getNextToken();
            REQUIRE(token.type == Type::LPAREN);
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

SCENARIO("Testing the absolute amount function and similarly named variables",
         "[lexer]") {
    GIVEN("The expression abs(absinth)") {
        Lexer lexer("abs(absinth)");
        THEN("first token is an operand, third token is a variable") {
            Token token = lexer.getNextToken();
            REQUIRE(token.type == Type::OP);
            lexer.getNextToken();
            token = lexer.getNextToken();
            REQUIRE(token.type == Type::VAR);
        }
    }
    GIVEN("The expression absinth + absolute") {
        Lexer lexer("absinth + absolute");
        THEN("first token is a variable, third token is a variable") {
            Token token = lexer.getNextToken();
            REQUIRE(token.type == Type::VAR);
            lexer.getNextToken();
            token = lexer.getNextToken();
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
