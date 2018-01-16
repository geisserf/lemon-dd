#ifndef STRING_UTILS_H
#define STRING_UTILS_H 

#include <algorithm> 
#include <cctype>
#include <locale>


#include "../evmdd/monoids/facts.h"

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



// trim from start (in place)
static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

};

#endif /* STRING_UTILS_H */
