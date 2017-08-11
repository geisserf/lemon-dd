#include "parser.h"
#include "catamorph/expression.h"
#include "catamorph/factories.h"

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

    // Iversion regex
    std::regex lSqrBrackRegex("\\[(.*)"); // Regex for [
    std::regex rSqrBrackRegex("\\](.*)"); // Regex for ]
    std::regex andRegex("\\&\\&(.*)");    // Regex for && (Logical and)
    std::regex orRegex("\\|\\|(.*)");     // Regex for || (Logical or)
    std::regex equalsRegex("\\=\\=(.*)"); // Regex for == (Logical equals)
    std::regex notRegex("\\!(.*)");       // Regex for ! (Logical not)

    Token token;
    if (std::regex_match(input, addRegex)) {
        token.type = Type::OP;
        token.value = "+";
        input = std::regex_replace(input, addRegex, "$1");
    } else if (std::regex_match(input, subRegex)) {
        token.type = Type::OP;
        token.value = "-";
        input = std::regex_replace(input, subRegex, "$1");
    } else if (std::regex_match(input, multRegex)) {
        token.type = Type::OP;
        token.value = "*";
        input = std::regex_replace(input, multRegex, "$1");
    } else if (std::regex_match(input, divRegex)) {
        token.type = Type::OP;
        token.value = "/";
        input = std::regex_replace(input, divRegex, "$1");
    } else if (std::regex_match(input, andRegex)) {
        token.type = Type::OP;
        token.value = "&&";
        input = std::regex_replace(input, andRegex, "$1");
    } else if (std::regex_match(input, orRegex)) {
        token.type = Type::OP;
        token.value = "||";
        input = std::regex_replace(input, orRegex, "$1");
    } else if (std::regex_match(input, equalsRegex)) {
        token.type = Type::OP;
        token.value = "==";
        input = std::regex_replace(input, equalsRegex, "$1");
    } else if (std::regex_match(input, notRegex)) {
        token.type = Type::OP;
        token.value = "!";
        input = std::regex_replace(input, notRegex, "$1");
    } else if (std::regex_match(input, constantRegex)) {
        token.type = Type::CONST;
        token.value = std::regex_replace(input, constantRegex, "$1");
        input = std::regex_replace(input, constantRegex, "$2");
    } else if (std::regex_match(input, lParenRegex)) {
        token.type = Type::LPAREN;
        input = std::regex_replace(input, lParenRegex, "$1");
    } else if (std::regex_match(input, rParenRegex)) {
        token.type = Type::RPAREN;
        input = std::regex_replace(input, rParenRegex, "$1");
    } else if (std::regex_match(input, lSqrBrackRegex)) {
        token.type = Type::LSQRBRACK;
        input = std::regex_replace(input, lSqrBrackRegex, "$1");
    } else if (std::regex_match(input, rSqrBrackRegex)) {
        token.type = Type::RSQRBRACK;
        input = std::regex_replace(input, rSqrBrackRegex, "$1");
    } else if (std::regex_match(input, varRegex)) {
        token.type = Type::VAR;
        token.value = std::regex_replace(input, varRegex, "$1");
        input = std::regex_replace(input, varRegex, "$2");
    } else if (input.empty()) {
        token.type = Type::END;
    }
    if (token.type == Type::INVALID) {
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
    case Type::CONST:
        return Factories::cst(stod(token.value));
        break;
    case Type::VAR:
        return Factories::var(token.value);
        break;
    case Type::OP:
        lexer.revert();
        return parseOpExpression(lexer);
        break;
    case Type::LPAREN: {
        string const beforeRParen = lexer.input;
        Expression expression = parseExpression(lexer);
        token = lexer.getNextToken();
        if (token.type != Type::RPAREN) {
            throw std::invalid_argument("Missing ) for substring " +
                                        beforeRParen);
        }
        return expression;
        break;
    }
    case Type::RPAREN:
        throw std::invalid_argument("No matching ( for substring " +
                                    lexer.input);
        break;
    case Type::LSQRBRACK: {
        string const beforeRParen = lexer.input;
        Expression expression = parseExpression(lexer);
        token = lexer.getNextToken();
        if (token.type != Type::RSQRBRACK) {
            throw std::invalid_argument("Missing ] for substring " +
                                        beforeRParen);
        }
        return expression;
        break;
    }
    case Type::RSQRBRACK:
        throw std::invalid_argument("No matching [ for substring " +
                                    lexer.input);
    default:
        throw std::invalid_argument("Illegal expression: " + lexer.input);
        break;
    }
}

Expression Parser::parseOpExpression(Lexer &lexer) const {
    Token token = lexer.getNextToken();
    assert(token.type == Type::OP);
    string opType = token.value;

    string const beforeRParen = lexer.input;
    token = lexer.getNextToken();
    vector<Expression> exprs;
    while (token.type != Type::RPAREN) {
        if (token.type == Type::END) {
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
    } else if (opType == "&&") {
        return Factories::land(exprs);
    } else if (opType == "||") {
        return Factories::lor(exprs);
    } else if (opType == "==") {
        return Factories::equals(exprs);
    } else if (opType == "!") {
        return Factories::lnot(exprs);
    } else {
        throw std::invalid_argument("Unknown operator:" + opType);
    }
}

bool InfixParser::isBinaryOperator(Token const &token) {
    return (token.value == "+" || token.value == "-" || token.value == "*" ||
            token.value == "/");
}

bool InfixParser::isUnaryOperator(Token const &token) {
    return token.value == "-";
}

bool InfixParser::hasHigherPrecedence(Token const &first, Token const &second) {
    return op_precedence.at(first.value) > op_precedence.at(second.value);
}

bool InfixParser::isLogicalBinaryOperator(Token const &token) {
    return (token.value == "&&" || token.value == "||" || token.value == "==");
}

bool InfixParser::isLogicalUnaryOperator(Token const &token) {
    return (token.value == "!");
}

void InfixParser::expect(Type type, Lexer &lexer) {
    if (type == next.type) {
        consume(lexer);
    } else {
        throw std::invalid_argument("Expected " + std::to_string(type) +
                                    " was " + std::to_string(next.type));
    }
}

void InfixParser::popOperator() {
    if (isBinaryOperator(operators.top()) ||
        isLogicalBinaryOperator(operators.top())) {
        Expression lhs = operands.top();
        operands.pop();
        Expression rhs = operands.top();
        operands.pop();
        operands.push(createExpression(rhs, operators.top(), lhs));
        operators.pop();
    } else {
        Expression lhs = operands.top();
        operands.pop();
        operands.push(createUnaryExpression(lhs, operators.top()));
        operators.pop();
    }
}

void InfixParser::pushOperator(Token const &token) {
    while (hasHigherPrecedence(operators.top(), token)) {
        popOperator();
    }
    operators.push(token);
}

void InfixParser::consume(Lexer &lexer) {
    next = lexer.getNextToken();
}

Expression InfixParser::parse(string const &input) {
    Lexer lexer(input);
    next = lexer.getNextToken();
    Token sentinel = Token(Type::OP, "sentinel");
    operators.push(sentinel);
    E(lexer);
    expect(Type::END, lexer);
    return operands.top();

    // return parseExpression(lexer);
}

void InfixParser::E(Lexer &lexer) {
    if (next.type == Type::LSQRBRACK) {
        LogicEXP(lexer);
    } else {
        P(lexer);
    }
    while (isBinaryOperator(next)) {
        pushOperator(next);
        consume(lexer);
        P(lexer);
    }

    while (operators.top().value != "sentinel") {
        popOperator();
    }
}

void InfixParser::P(Lexer &lexer) {
    if (next.type == Type::VAR) {
        operands.push(Factories::var(next.value));
        consume(lexer);
    } else if (next.type == Type::CONST) {
        operands.push(Factories::cst(stod(next.value)));
        consume(lexer);
    } else if (next.type == Type::LPAREN) {
        consume(lexer);
        Token sentinel = Token(Type::OP, "sentinel");
        operators.push(sentinel);
        E(lexer);
        expect(Type::RPAREN, lexer);
        operators.pop();
    } else if (isUnaryOperator(next)) {
        pushOperator(next);
        consume(lexer);
        P(lexer);
    } else if (next.type == Type::LSQRBRACK) {
        LogicEXP(lexer);
    } else {
        throw std::invalid_argument(
            "P Unknown Token \"" + std::to_string(next.type) +
            "\" with value: \"" + next.value + "\" at " + lexer.input);
    }
}

void InfixParser::LogicEXP(Lexer &lexer) {
    if (next.type != Type::LSQRBRACK) {
        throw std::invalid_argument("exprected [");
    }
    Token sentinel = Token(Type::OP, "sentinel");
    operators.push(sentinel);
    consume(lexer); // consume [
    LP(lexer);
    while (isLogicalBinaryOperator(next)) {
        std::cout << "Binary: " << next.value << std::endl;
        pushOperator(next);
        consume(lexer);
        LP(lexer);
    }

    expect(Type::RSQRBRACK, lexer);

    while (operators.top().value != "sentinel") {
        popOperator();
    }
}

void InfixParser::LP(Lexer &lexer) {
    if (next.type == Type::VAR) {
        std::cout << "VAR: " << next.value << std::endl;
        operands.push(Factories::var(next.value));
        consume(lexer);
    } else if (next.type == Type::CONST) {
        std::cout << "Const: " << next.value << std::endl;
        operands.push(Factories::cst(stod(next.value)));
        consume(lexer);
    } else if (isLogicalUnaryOperator(next)) {
        std::cout << "L Unary: " << next.value << std::endl;
        pushOperator(next);
        consume(lexer);
        LP(lexer);
    } else if (next.type == Type::LSQRBRACK) {
        std::cout << "New Exp: " << next.value << std::endl;
        LogicEXP(lexer);
    } else {
        throw std::invalid_argument(
            " LP Unknown Token \"" + std::to_string(next.type) +
            "\" with value \"" + next.value + "\" at " + lexer.input);
    }
}

/*
Expression InfixParser::parseExpression(Lexer &lexer) const {
    Expression lhs = parseTerm(lexer);
    Token token = lexer.getNextToken();

    switch (token.type) {
    case Token::OP: {
        // expression is a binary expression
        Expression rhs = parseTerm(lexer);
        Token next_token = lexer.getNextToken();
        lexer.revert();
        if (next_token.type != Token::END && next_token.type != Token::RPAREN &&
            next_token.type != Token::RSQRBRACK) {
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
    case Token::RSQRBRACK: {
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
        if (token.value == "!") {
            return Factories::lnot({parseExpression(lexer)});
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
    case Token::RPAREN: {
        throw std::invalid_argument("No matching ( for substring " +
                                    lexer.input);
        break;
    }
    case Token::LSQRBRACK: {
        string const beforeRParen = lexer.input;
        Expression expression = parseExpression(lexer);
        token = lexer.getNextToken();
        if (token.type != Token::RSQRBRACK) {
            throw std::invalid_argument("Missing ] for substring " +
                                        beforeRParen);
        }
        return expression;
        break;
    }
    case Token::RSQRBRACK: {
        throw std::invalid_argument("No matching [ for substring " +
                                    lexer.input);
        break;
    }
    default:
        throw std::invalid_argument("Illegal factor: " + lexer.input);
        break;
    }
}
*/
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
    } else if (op.value == "&&") {
        return Factories::land(exprs);
    } else if (op.value == "||") {
        return Factories::lor(exprs);
    } else if (op.value == "==") {
        return Factories::equals(exprs);
    } else if (op.value == "!") {
        return Factories::lnot(exprs);
    } else {
        throw std::invalid_argument("Unknown binary operator:" + op.value);
    }
}

Expression InfixParser::createUnaryExpression(Expression const &exp,
                                              Token op) const {
    if (op.value == "!") {
        vector<Expression> exprs{exp};
        return Factories::lnot(exprs);
    } else if (op.value == "-") {
        vector<Expression> exprs{Factories::cst(0), exp};
        return Factories::sub(exprs);
    } else {
        throw std::invalid_argument("Unknown unary operator:" + op.value);
    }
}
