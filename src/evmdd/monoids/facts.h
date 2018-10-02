#ifndef VARIABLE_ASSIGNMENT_H
#define VARIABLE_ASSIGNMENT_H

#include <algorithm>
#include <string>
#include <set>
#include <string>

// A fact is a variable-value pair
using Fact = std::pair<std::string, int>;
using Facts = std::set<Fact>;

struct Union {
    Facts operator()(const Facts &lhs, const Facts &rhs) {
        Facts result;
        std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                       std::inserter(result, result.end()));
        return result;
    }
};

#endif /* VARIABLE_ASSIGNMENT_H */
