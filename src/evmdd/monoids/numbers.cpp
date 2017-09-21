#include "../monoid.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

using std::vector;

// Monoids for built-in number data types like int, float, double

template <typename M>
Monoid<M> Monoid<M>::greatest_lower_bound(vector<Monoid<M>> const &subset) {
    // For greatest lower bound we can use min_element with <
    auto min = *std::min_element(subset.begin(), subset.end());
    return min;
}

template <typename M>
Monoid<M> Monoid<M>::neutral_element() {
    return {0};
}

template <typename M>
std::string Monoid<M>::to_string() const {
    // We use the stream operator to print e instead of std::to_string,
    // because then we can set the precision before we print (otherwise
    // even integer constant n is printed as n.000000
    std::ostringstream out;
    out << value;
    return out.str();
}

template class Monoid<int>;
template class Monoid<float>;
template class Monoid<double>;

