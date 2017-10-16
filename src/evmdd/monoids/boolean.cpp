#include "boolean.h"


// Monoid over <true, false> with logical_or as operator.

// Monus for boolean monoids is a && !b
template <>
BoolMonoid& BoolMonoid::operator-=(BoolMonoid const& rhs) {
    value = std::logical_and<bool>()(value, std::logical_not<bool>()(rhs.value));
    return *this;
}

// Greatest lower bound for boolean monoids is &&
template <>
bool BoolMonoid::greatest_lower_bound(bool const& l, bool const& r) {
    return std::logical_and<bool>()(l,r);
}

template <>
bool BoolMonoid::neutral_element() {
    return false;
}

template <>
std::string BoolMonoid::to_string() const {
    return value ? "T" : "F";
}
