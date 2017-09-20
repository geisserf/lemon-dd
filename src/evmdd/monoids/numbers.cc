#include "../monoid.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

// Monoids for built-in number data types like int, float, double

template <typename M>
bool less(Monoid<M> const &l, Monoid<M> const &r) {
    return l.get_value() <= r.get_value();
}

template <typename M>
template <typename Container>
M Monoid<M>::greatest_lower_bound(Container const &subset) const {
    // For greatest lower bound we can use min_element with <
    return *std::min_element(std::begin(subset), std::end(subset));
}

template <typename M>
M constexpr Monoid<M>::neutral_element() {
    return 0;
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

