#ifndef MONOID_H
#define MONOID_H

#include <functional>
#include <set>
#include <string>
#include <vector>

#include <boost/functional/hash.hpp>

/* This is the base class for monoids over carrier set M. A monoid has a binary
 * associative operation '+' over M and a neutral element. Additionally, these
 * monoids are semi-lattice ordered, i.e. there exists a greatest lower bound
 * operator, defined such the greatest lower bound of the whole carrier set M
 * is the neutral element. Last, a monoid has to support a monus, which
 * basically is the '-' equivalent to '+'.
 *
 * Implementations for particular monoids can be found in monoids/. To define
 * a new monoid, we have to know the carrier set (M) and the binary associative
 * operator+ (F) and its monus '-'. Then we only have to write the 
 * specialization for neutral_element() and greatest_lower_bound(). 
 * If F is not specified, it is by default std::plus<M>.
 *
 * One particular implementation is the product of two monoids L,R = LxR with
 * '+' of L given as function F, '+' of R as G, defined in monoids/product.h,
 * which allows to create the product of any two monoids.
 */

// Required for non-inlined friend definition of less,
// see https://stackoverflow.com/questions/4660123/
template <typename M, typename F = std::plus<M>>
class Monoid;

template <typename M, typename F>
bool less(Monoid<M, F> const &l, Monoid<M, F> const &r);

template <typename M, typename F>
class Monoid {
public:
    // Default constructor is the neutral element
    Monoid() : value(neutral_element()) {}
    explicit Monoid(M value) : value(value) {}
    // explicit Monoid(Monoid<M,F> const& other) : value(other.value) {}

    // Binary associative operator '+' over M calls Functor F.
    Monoid<M, F> &operator+=(Monoid<M, F> const &rhs) {
        value = F()(value, rhs.value);
        return *this;
    }

    friend Monoid operator+(Monoid lhs, Monoid const &rhs) {
        lhs += rhs;
        return lhs;
    }

    // Monus operator '-'
    Monoid<M, F> &operator-=(Monoid<M, F> const &rhs);

    friend Monoid operator-(Monoid lhs, Monoid const &rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend bool operator<(Monoid const &l, Monoid const &r) {
        return l.value < r.value;
    }

    friend bool operator==(Monoid const &l, Monoid const &r) {
        return l.value == r.value;
    }

    // Greatest lower bound such that greatest_lower_bound(M) = '0'.
    static M greatest_lower_bound(M const &l, M const &r);

    // Returns the value associated with this monoid object.
    M get_value() const {
        return value;
    }

    // Neutral element '0' such that m + '0' = m for every m in M.
    // This is not constexpr, since M may be a non-literal type. See
    // http://en.cppreference.com/w/cpp/concept/LiteralType
    static M neutral_element();

    // Prints the value
    std::string to_string() const;

private:
    M value;
};

// Required to calculate hash values of monoids
template <typename M, typename F>
std::size_t hash_value(Monoid<M,F> const &monoid) {
    return boost::hash<M>{}(monoid.get_value());
}


#include "monoids/product.h"
#include "monoids/boolean.h"
#include "monoids/variable_assignment.h"

#endif /* MONOID_H */

