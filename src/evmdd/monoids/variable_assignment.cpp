#include "../monoid.h"

#include <algorithm>
#include <cassert>
#include <set>

using std::vector;

// Monoid over the set of facts 2^F

// A fact is a variable-value pair
using Fact = std::pair<std::string, int>;
using Facts = std::set<Fact>;

// greatest lower bound is set intersection between all sets
template <>
Monoid<Facts> Monoid<Facts>::greatest_lower_bound(
    vector<Monoid<Facts>> const &monoids) {
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

template <>
Monoid<Facts> Monoid<Facts>::neutral_element() {
    // neutral element is the empty set
    auto result = std::set<Fact>{};
    return result;
}

template <>
std::string Monoid<Facts>::to_string() const {
    std::string result = "{";
    for (Fact const &f : value) {
        std::string fact_string = f.first + "=" + std::to_string(f.second);
        result += " " + fact_string;
    }
    return result + "}";
}
