#include "parser.h"
#include "catamorph/Expression.h"
#include "catamorph/Factories.h"

#include <assert.h>
#include <iostream>
#include <regex>

using std::string;
using std::vector;
using std::shared_ptr;
using std::stod;

Token Lexer::getNextToken() {
    if (returnPrevToken) {
        returnPrevToken = false;
        return previousToken;
    }

    // trim surrounding parantheses
    trim(input);

    std::regex addRegex("\\+(.*)");  // Regex for arithmetic +
    std::regex subRegex("-(.*)");    // Regex for arithmetic -
    std::regex multRegex("\\*(.*)"); // Regex for arithmetic *
    std::regex divRegex("\\/(.*)");  // Regex for arithmetic /
    // Regex for constant numbers.
    // Note that we use the passive group (?:subpattern) here, so that we only
    // have one backreference for the whole constant and not multiple
    // backreferences for individual constant parts
    std::regex constantRegex(
        "((?:[[:digit:]]+)(?:\\.(?:(?:[[:digit:]]+)?))?)(.*)");
    std::regex lParenRegex("\\((.*)"); // Regex for (
    std::regex rParenRegex("\\)(.*)"); // Regex for )
    // Variables contain letters, numbers or "_",
    // but must not start with a number
    std::regex varRegex("((?:[[:alpha:]|_]+)(?:[[:alnum:]|_]*))(.*)");

    Token token;
    if (std::regex_match(input, addRegex)) {
        token.type = Token::OP;
        token.value = "+";
        input = std::regex_replace(input, addRegex, "$1");
    } else if (std::regex_match(input, subRegex)) {
        token.type = Token::OP;
        token.value = "-";
        input = std::regex_replace(input, subRegex, "$1");
    } else if (std::regex_match(input, multRegex)) {
        token.type = Token::OP;
        token.value = "*";
        input = std::regex_replace(input, multRegex, "$1");
    } else if (std::regex_match(input, divRegex)) {
        token.type = Token::OP;
        token.value = "/";
        input = std::regex_replace(input, divRegex, "$1");
    } else if (std::regex_match(input, constantRegex)) {
        token.type = Token::CONST;
        token.value = std::regex_replace(input, constantRegex, "$1");
        input = std::regex_replace(input, constantRegex, "$2");
    } else if (std::regex_match(input, lParenRegex)) {
        token.type = Token::LPAREN;
        input = std::regex_replace(input, lParenRegex, "$1");
    } else if (std::regex_match(input, rParenRegex)) {
        token.type = Token::RPAREN;
        input = std::regex_replace(input, rParenRegex, "$1");
    } else if (std::regex_match(input, varRegex)) {
        token.type = Token::VAR;
        token.value = std::regex_replace(input, varRegex, "$1");
        input = std::regex_replace(input, varRegex, "$2");
    } else if (input.empty()) {
        token.type = Token::END;
    }
    if (token.type == Token::INVALID) {
        string error = "Illegal token at start of substring: \"" + input;
        error += "\"";
        throw std::invalid_argument(error);
        exit(0);
    }
    previousToken = token;
    return token;
}

// Trim spaces from start
std::string &Lexer::ltrim(std::string &s) {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// Trim spaces from end
std::string &Lexer::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace)))
                .base(),
            s.end());
    return s;
}

// Trim spaces from both ends
std::string &Lexer::trim(std::string &s) {
    return ltrim(rtrim(s));
}

Expression Parser::parse(string const &input) const {
    // For user convenience we surround the input string with parantheses
    string expr = "(" + input + ")";
    Lexer lexer(expr);
    return parseExpression(lexer);
}

Expression Parser::parseExpression(Lexer &lexer) const {
    Token token = lexer.getNextToken();
    switch (token.type) {
    case Token::CONST:
        return Factories::cst(stod(token.value));
        break;
    case Token::VAR:
        return Factories::var(token.value);
        break;
    case Token::OP:
        lexer.revert();
        return parseOpExpression(lexer);
        break;
    case Token::LPAREN: {
        string const beforeRParen = lexer.input;
        Expression expression = parseExpression(lexer);
        token = lexer.getNextToken();
        if (token.type != Token::RPAREN) {
            throw std::invalid_argument("Missing ) for substring " +
                                        beforeRParen);
        }
        return expression;
        break;
    }
    case Token::RPAREN:
        throw std::invalid_argument("No matching ( for substring " +
                                    lexer.input);
        break;
    default:
        throw std::invalid_argument("Illegal expression: " + lexer.input);
        break;
    }
}

Expression Parser::parseOpExpression(Lexer &lexer) const {
    Token token = lexer.getNextToken();
    assert(token.type == Token::OP);
    string opType = token.value;

    string const beforeRParen = lexer.input;
    token = lexer.getNextToken();
    vector<Expression> exprs;
    while (token.type != Token::RPAREN) {
        if (token.type == Token::END) {
            throw std::invalid_argument("Missing ) for substring " +
                                        beforeRParen);
        }
        lexer.revert();
        exprs.push_back(parseExpression(lexer));
        token = lexer.getNextToken();
    }
    if (exprs.empty()) {
        throw std::invalid_argument("Empty operator: " + opType + " before" +
                                    beforeRParen);
    }
    // Revert because the ")" is again checked in the "(" expression part
    lexer.revert();
    if (opType == "+") {
        return Factories::add(exprs);
    } else if (opType == "-") {
        return Factories::sub(exprs);
    } else if (opType == "*") {
        return Factories::mul(exprs);
    } else {
        throw std::invalid_argument("Unknown operator:" + opType);
    }
}

Expression InfixParser::parse(string const &input) const {
    Lexer lexer(input);
    return parseExpression(lexer);
}

Expression InfixParser::parseExpression(Lexer &lexer) const {
    Expression lhs = parseTerm(lexer);
    Token token = lexer.getNextToken();

    switch (token.type) {
    case Token::OP: {
        // expression is a binary expression
        Expression rhs = parseTerm(lexer);
        Token next_token = lexer.getNextToken();
        lexer.revert();
        if (next_token.type != Token::END && next_token.type != Token::RPAREN) {
            throw std::invalid_argument(
                "Expected expression to end or ')' for " + lexer.input);
        }
        return createExpression(lhs, token, rhs);
        break;
    }
    case Token::END: {
        // expression is a single term
        return lhs;
        break;
    }
    case Token::RPAREN: {
        // expression is part of a factor, leave syntax check to factor function
        lexer.revert();
        return lhs;
        break;
    }
    default:
        throw std::invalid_argument("Illegal expression: " + lexer.input +
                                    "\n" + "token value: " + token.value);
        break;
    }
}

Expression InfixParser::parseTerm(Lexer &lexer) const {
    // term = factor { ("*" | "/") factor}
    Expression lhs = parseFactor(lexer);
    Token token = lexer.getNextToken();

    // TODO this is a little hacky, probably better to have two types of
    // operand tokens (+,- and *,/)
    if (token.type == Token::OP && (token.value == "*" || token.value == "/")) {
        // expression is a binary expression
        Expression rhs = parseFactor(lexer);
        return createExpression(lhs, token, rhs);
    } else {
        // Syntax checking is handled by parent caller
        lexer.revert();
        return lhs;
    }
}

Expression InfixParser::parseFactor(Lexer &lexer) const {
    // factor = {"+" | "-" } (constant |  variable | "(" expression ")")
    Token token = lexer.getNextToken();
    switch (token.type) {
    case Token::CONST:
        return Factories::cst(stod(token.value));
        break;
    case Token::VAR: {
        return Factories::var(token.value);
        break;
    }
    case Token::OP: {
        if (token.value == "+") {
            return Factories::add({Factories::cst(0), parseExpression(lexer)});
        }
        if (token.value == "-") {
            return Factories::sub({Factories::cst(0), parseExpression(lexer)});
        }
        throw std::invalid_argument("Expected unary operator in factor " +
                                    lexer.input);
        break;
    }
    case Token::LPAREN: {
        string const beforeRParen = lexer.input;
        Expression expression = parseExpression(lexer);
        token = lexer.getNextToken();
        if (token.type != Token::RPAREN) {
            throw std::invalid_argument("Missing ) for substring " +
                                        beforeRParen);
        }
        return expression;
        break;
    }
    case Token::RPAREN:
        throw std::invalid_argument("No matching ( for substring " +
                                    lexer.input);
        break;
    default:
        throw std::invalid_argument("Illegal factor: " + lexer.input);
        break;
    }
}

Expression InfixParser::createExpression(Expression const &lhs, Token op,
                                         Expression const &rhs) const {
    vector<Expression> exprs{lhs, rhs};
    if (op.value == "+") {
        return Factories::add(exprs);
    } else if (op.value == "-") {
        return Factories::sub(exprs);
    } else if (op.value == "*") {
        return Factories::mul(exprs);
    } else if (op.value == "/") {
        return Factories::div(exprs);
    } else {
        throw std::invalid_argument("Unknown operator:" + op.value);
    }
}
