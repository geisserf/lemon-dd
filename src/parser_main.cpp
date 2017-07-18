#include "parser.h"
#include "catamorph/expression.h"
#include "catamorph/printer.h"

#include <iostream>
#include <string>

using std::string;

int main() {
    string input;
    std::cout << "Type in arithmetic expression." << std::endl;
    std::getline(std::cin, input);
    InfixParser parser;
    std::cout << "Trying to parse " << input << std::endl;
    Expression expression = parser.parse(input);
    string result = Printer::asString(expression);
    std::cout << result << std::endl;
}
