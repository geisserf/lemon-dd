#include "logical_formula.h"
#include "catamorph/printer.h"
#include "parser.h"

Formula::Formula(std::string const &input) {
    InfixParser parser;
    expression = parser.parse(input);
}

void Formula::print() const {
    Printer::print(expression);
}

std::string Formula::to_string() const {
    return Printer::as_string(expression);
}
