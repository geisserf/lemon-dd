#include "../monoid.h"

#include <algorithm>
#include <cassert>

using std::vector;

// Monoid over the set of facts 2^F with set union as operator.

// Monus is set minus
template <>
Monoid<Facts, Union> &Monoid<Facts, Union>::operator-=(
    Monoid<Facts, Union> const &rhs) {
    Facts diff;
    std::set_difference(value.begin(), value.end(), rhs.value.begin(),
                        rhs.value.end(), std::inserter(diff, diff.begin()));
    value = diff;
    return *this;
}

// Greatest lower bound is set intersection
template <>
Facts Monoid<Facts, Union>::greatest_lower_bound(Facts const &l,
                                                 Facts const &r) {
    Facts result;
    std::set_intersection(l.begin(), l.end(), r.begin(), r.end(),
                          std::inserter(result, result.end()));
    return result;
}

// Neutral element is the empty set
template <>
Facts Monoid<Facts, Union>::neutral_element() {
    return std::set<Fact>{};
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
