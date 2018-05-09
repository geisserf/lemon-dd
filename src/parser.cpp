#include "parser.h"
#include "catamorph/expression.h"
#include "catamorph/factories.h"
#include "utils/string_utils.h"

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
    StringUtils::trim(input);

    std::regex addRegex("\\+(.*)");     // Regex for arithmetic +
    std::regex subRegex("-(.*)");       // Regex for arithmetic -
    std::regex multRegex("\\*(.*)");    // Regex for arithmetic *
    std::regex divRegex("\\/(.*)");     // Regex for arithmetic /
    std::regex greaterRegex("\\>(.*)"); // Regex for > comparison
    std::regex lesserRegex("\\<(.*)"); // Regex for < comparison
    std::regex greater_equals_Regex("\\>\\=(.*)"); // Regex for >= comparison
    std::regex lesser_equals_Regex("\\<\\=(.*)"); // Regex for <= comparison
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
    std::regex varRegex("((?:[[:alpha:]_]+)(?:[[:alnum:]_]*))(.*)");

    // Iverson regex
    std::regex lSqrBrackRegex("\\[(.*)"); // Regex for [
    std::regex rSqrBrackRegex("\\](.*)"); // Regex for ]
    std::regex andRegex("\\&\\&(.*)");    // Regex for && (Logical and)
    std::regex orRegex("\\|\\|(.*)");     // Regex for || (Logical or)
    std::regex equalsRegex("\\=\\=(.*)"); // Regex for == (Logical equals)
    std::regex notRegex("\\!(.*)");       // Regex for ! (Logical not)

    // Absolute amount function regex: abs(x)
    // TODO variables with the word abs may be seen as absolute amount.
    // A possible fix is to check for the paranthese after abs.
    std::regex absRegex("abs(.*)");

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
    } else if (std::regex_match(input, greater_equals_Regex)) {
        token.type = Type::OP;
        token.value = ">=";
        input = std::regex_replace(input, greater_equals_Regex, "$1");
    } else if (std::regex_match(input, lesser_equals_Regex)) {
        token.type = Type::OP;
        token.value = "<=";
        input = std::regex_replace(input, lesser_equals_Regex, "$1");
    } else if (std::regex_match(input, greaterRegex)) {
        token.type = Type::OP;
        token.value = ">";
        input = std::regex_replace(input, greaterRegex, "$1");
    } else if (std::regex_match(input, lesserRegex)) {
        token.type = Type::OP;
        token.value = "<";
        input = std::regex_replace(input, lesserRegex, "$1");
    } else if (std::regex_match(input, absRegex)) {
        token.type = Type::OP;
        token.value = "abs";
        input = std::regex_replace(input, absRegex, "$1");
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

/*
 * PREFIX PARSER
 */

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
    } else if (opType == ">") {
        return Factories::greater(exprs);
    } else if (opType == "<") {
        return Factories::lesser(exprs);
    } else if (opType == ">=") {
        return Factories::greater_equals(exprs);
    } else if (opType == "<=") {
        return Factories::lesser_equals(exprs);
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

/*
 * INFIX PARSER
 */

bool InfixParser::isBinaryOperator(Token const &token) {
    return (token.value == "+" || token.value == "-" || token.value == "*" ||
            token.value == "/" || token.value == ">" || token.value == ">=" ||
            token.value == "<" || token.value == "<=");
}

bool InfixParser::isUnaryOperator(Token const &token) {
    return (token.value == "-" || token.value == "abs");
}

bool InfixParser::hasHigherPrecedence(Token const &first, Token const &second) {
    if (!second.binary && second.value == "-") {
        return false;
    }
    if (!first.binary && first.value == "-") {
        return true;
    }

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
        throw std::invalid_argument("Parser error : expected " +
                                    std::to_string(type) + " was " +
                                    std::to_string(next.type));
    }
}

void InfixParser::popOperator() {
    if ((isBinaryOperator(operators.top()) ||
         isLogicalBinaryOperator(operators.top())) &&
        operators.top().binary) {
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
        // std::cout<< "VAR: "<<next.value<<std::endl;
        operands.push(Factories::var(next.value));
        consume(lexer);
    } else if (next.type == Type::CONST) {
        // std::cout<< "Const: "<<next.value<<std::endl;
        operands.push(Factories::cst(stod(next.value)));
        consume(lexer);
    } else if (next.type == Type::LPAREN) {
        // std::cout<< "LPAREN: "<<next.value<<std::endl;
        consume(lexer);
        Token sentinel = Token(Type::OP, "sentinel");
        operators.push(sentinel);
        E(lexer);
        expect(Type::RPAREN, lexer);
        operators.pop();
    } else if (isUnaryOperator(next)) {
        // std::cout<< "unary: "<<next.value<<std::endl;
        next.binary = false;
        pushOperator(next);
        consume(lexer);
        P(lexer);
    } else if (next.type == Type::LSQRBRACK) {
        // std::cout<< "L Exp: "<<next.value<<std::endl;
        LogicEXP(lexer);
    } else {
        throw std::invalid_argument(
            "P Unknown Token \"" + std::to_string(next.type) +
            "\" with value: \"" + next.value + "\" at " + lexer.input);
    }
}

void InfixParser::LogicEXP(Lexer &lexer) {
    if (next.type != Type::LSQRBRACK) {
        throw std::invalid_argument("expected [");
    }
    Token sentinel = Token(Type::OP, "sentinel");
    operators.push(sentinel);
    consume(lexer); // consume [
    LP(lexer);
    while (isLogicalBinaryOperator(next)) {
        pushOperator(next);
        consume(lexer);
        LP(lexer);
    }
    expect(Type::RSQRBRACK, lexer);
    while (operators.top().value != "sentinel") {
        popOperator();
    }
    operators.pop(); // remove [
}

void InfixParser::LP(Lexer &lexer) {
    if (next.type == Type::VAR) {
        operands.push(Factories::var(next.value));
        consume(lexer);
    } else if (next.type == Type::CONST) {
        operands.push(Factories::cst(stod(next.value)));
        consume(lexer);
    } else if (isLogicalUnaryOperator(next)) {
        next.binary = false;
        pushOperator(next);
        consume(lexer);
        LP(lexer);
    } else if (next.type == Type::LSQRBRACK) {
        LogicEXP(lexer);
    } else if (next.type == Type::LPAREN) {
        consume(lexer);
        Token sentinel = Token(Type::OP, "sentinel");
        operators.push(sentinel);
        E(lexer);
        expect(Type::RPAREN, lexer);
        operators.pop();
    } else {
        throw std::invalid_argument(
            " LP Unknown Token \"" + std::to_string(next.type) +
            "\" with value \"" + next.value + "\" at " + lexer.input);
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
    } else if (op.value == ">") {
        return Factories::greater(exprs);
    } else if (op.value == "<") {
        return Factories::lesser(exprs);
    } else if (op.value == ">=") {
        return Factories::greater_equals(exprs);
    } else if (op.value == "<=") {
        return Factories::lesser_equals(exprs);
    } else if (op.value == "&&") {
        return Factories::land(exprs);
    } else if (op.value == "||") {
        return Factories::lor(exprs);
    } else if (op.value == "==") {
        return Factories::equals(exprs);
    } else {
        throw std::invalid_argument("Unknown binary operator:" + op.value);
    }
}

Expression InfixParser::createUnaryExpression(Expression const &exp,
                                              Token op) const {
    if (op.value == "!") {
        return Factories::lnot({exp});
    } else if (op.value == "-") {
        vector<Expression> exprs{Factories::cst(0), exp};
        return Factories::sub(exprs);
    } else if (op.value == "abs") {
        return Factories::abs({exp});
    } else {
        throw std::invalid_argument("Unknown unary operator:" + op.value);
    }
}
