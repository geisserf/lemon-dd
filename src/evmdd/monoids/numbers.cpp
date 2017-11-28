#include "../monoid.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

using std::vector;

// Monoids for built-in number data types like int, float, double

template <typename M, typename F>
Monoid<M, F> & Monoid<M,F>::operator-=(Monoid<M, F> const &rhs) {
    value -= rhs.value;
    return *this;
}

template <typename M, typename F>
M Monoid<M,F>::greatest_lower_bound(M const& l, M const& r) {
    // For greatest lower bound we can use min with <
    return std::min(l, r);
}

template <typename M, typename F>
M Monoid<M,F>::neutral_element() {
    return 0;
}

template <typename M, typename F>
std::string Monoid<M,F>::to_string() const {
    // We use the stream operator to print e instead of std::to_string,
    // because then we can set the precision before we print (otherwise
    // even integer constant n is printed as n.000000
    std::ostringstream out;
    out << value;
    return out.str();
}

template class Monoid<int, std::plus<int>>;
template class Monoid<float, std::plus<float>>;
template class Monoid<double, std::plus<double>>;

