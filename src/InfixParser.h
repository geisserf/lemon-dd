#ifndef NUMERIC_CATAMORPH_INFIXPARSER_H
#define NUMERIC_CATAMORPH_INFIXPARSER_H

#include <string>
#include <vector>
#include "catamorph/Expression.h"

class InfixParser {
private:
    std::string expression;
    std::size_t currentIndex;
    std::string getNextToken();


public:
    Expression parse(std::string expression);

};


#endif //NUMERIC_CATAMORPH_INFIXPARSER_H
