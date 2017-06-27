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

    // StringUtils::trim(input);

    std::regex addRegex("\\+([^[:alnum:]](.*))"); // Regex for arithmetic +
    std::regex subRegex("-([^[:alnum:]](.*))");   // Regex for arithmetic -
    std::regex multRegex("\\*(.*)");              // Regex for arithmetic *
    // Regex for constants. Constants are a number, optionally preceded by + or
    // -.
    // Note that we use the passive group (?:subpattern) here, so that we only
    // have one backreference for the whole constant and not multiple
    // backreferences for individual constant parts
    std::regex constantRegex(
        "((?:(?:\\+|-)?[[:digit:]]+)(?:\\.(?:(?:[[:digit:]]+)?))?)(.*)");
    std::regex lParenRegex("\\((.*)"); // Regex for (
    std::regex rParenRegex("\\)(.*)"); // Regex for )
    // Variables start with a letter or "_" character and can be positive or
    // negative
    std::regex varRegex("((?:\\+|-)?(?:[[:alpha:]]|_)+)(.*)");

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
        // Handle unary operator before variables (e.g. -A, +var)
        if (token.value[0] == '+') {
            return Factories::var(token.value.substr(1));
        } else if (token.value[0] == '-') {
            return Factories::sub(
                {Factories::cst(0), Factories::var(token.value.substr(1))});
        }
        return Factories::var(token.value);
        break;
    case Token::OP:
        lexer.revert();
        return parseOpExpression(lexer);
        break;
    case Token::LPAREN: {
        string const beforeRParen = lexer.input;
        token = lexer.getNextToken();
        lexer.revert();
        auto result = parseExpression(lexer);
        token = lexer.getNextToken();
        if (token.type != Token::RPAREN) {
            throw std::invalid_argument("Missing ) for substring " +
                                        beforeRParen);
        }
        return result;
        break;
    }
    case Token::RPAREN:
        throw std::invalid_argument("No matching ( for substring " +
                                    lexer.input);
        break;
    default:
        throw std::invalid_argument("Illegal expression");
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
