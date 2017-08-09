#ifndef PARSER_H
#define PARSER_H

#include <string>

struct Token {
    enum Type {
        OP,
        CONST,
        LPAREN,
        RPAREN,
        LSQRBRACK,
        RSQRBRACK,
        VAR,
        END,
        INVALID
    };

    Token(Type _type = INVALID, std::string val = "")
        : type(_type), value(val) {}

    Type type;

    // Integer value for constants, name for variables, operator name for
    // operators, empty string otherwise
    std::string value;
};

// Generates tokens from a given string
class Lexer {
public:
    Lexer(std::string const &s)
        : input(s), previousToken(Token(Token::END)), returnPrevToken(false) {}

    // Generates the next token and removes it from the input string
    Token getNextToken();

    // After revert the next token returned will be the previous token
    void revert() {
        returnPrevToken = true;
    }

    // Input string, modified during token generation
    std::string input;

private:
    // The previous token in case we want to "unget" the current token, maybe
    // because we went too far
    Token previousToken;
    bool returnPrevToken;

    // Trim spaces from start
    std::string &ltrim(std::string &s);

    // Trim spaces from end
    std::string &rtrim(std::string &s);

    // Trim spaces from both ends
    std::string &trim(std::string &s);
};

class Expression;

class Parser {
    // EBNF of our context free grammar:
    // sign = "+" | "-"
    // op =  sign | "*" | "/" | "&&" | "||" | "!" | "=="
    // digit = "0"|...|"9"
    // number = [sign] {digit}[.{digit}]
    // variable = [{digit}]{"a"|"A",...,"z"|"Z"|"_"}[{digit}]
    // expression = number | [sign] variable | "(" op {expression} ")"
    // paranthExpression = "(" expression ")"

public:
    Expression parse(std::string const &input) const;

private:
    Expression parseExpression(Lexer &lexer) const;
    Expression parseOpExpression(Lexer &lexer) const;
};

class InfixParser {
    // EBNF
    // expression = [(] term { ("+" | "-") term} [)]
    // term = factor { ("*" | "/") factor}
    // factor = {"+" | "-" } (constant |  variable | "(" expression ")")
    // variable = [{"+"|"-"}] {"a"|"A",...,"z"|"Z"|"_"}[{digit}]
    // number = [{"+"|"-"}]{digit}[.{digit}]
    // digit = "0"|...|"9"

public:
    // Parses the string and returns an expression or throws an error
    Expression parse(std::string const &input) const;

private:
    // Parse functions for BNF parts
    Expression parseExpression(Lexer &lexer) const;
    Expression parseTerm(Lexer &lexer) const;
    Expression parseFactor(Lexer &lexer) const;
    // Creates an expression of type op(lhs, rhs)
    Expression createExpression(Expression const &lhs, Token op,
                                Expression const &rhs) const;
};

#endif /* PARSER_H */
