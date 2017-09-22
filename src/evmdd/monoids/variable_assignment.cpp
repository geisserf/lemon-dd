#include "../monoid.h"

#include <algorithm>
#include <cassert>

using std::vector;

// Monoid over the set of facts 2^F with set union as operator.

// Greatest lower bound is set intersection between all sets
template <>
Monoid<Facts, Union> Monoid<Facts, Union>::greatest_lower_bound(
    vector<Monoid<Facts, Union>> const &monoids) {
    assert(!monoids.empty());
    Facts current = monoids[0].value;
    Facts next;
    for (size_t i = 1; i < monoids.size(); ++i) {
        std::set_intersection(current.begin(), current.end(),
                              monoids[i].value.begin(), monoids[i].value.end(),
                              std::inserter(next, next.end()));
        current = next;
        next.clear();
    }
    return {current};
}

// Neutral element is the empty set
template <>
Monoid<Facts, Union> Monoid<Facts,Union>::neutral_element() {
    auto result = std::set<Fact>{};
    return result;
}

template <>
std::string Monoid<Facts, Union>::to_string() const {
    std::string result = "{";
    std::string token = "";
    for (Fact const &f : value) {
        result += token + f.first + "=" + std::to_string(f.second);
        token = " ";
    }
    return result + "}";
}
