#ifndef STRING_UTILS_H
#define STRING_UTILS_H 

#include "../evmdd/monoids/variable_assignment.h"

struct StringUtils {

static std::string fact_printer(Fact const &fact) {
    return fact.first + "=" + std::to_string(fact.second);
}

static std::string fact_printer(Facts const &facts) {
    std::string result = "{";
    std::string sep = "";
    for (auto const &fact : facts) {
        result += sep + fact_printer(fact);
        sep = " ";
    }
    result += "}";
    return result;
}



};

#endif /* STRING_UTILS_H */
