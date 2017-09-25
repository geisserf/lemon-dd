#ifndef MONOID_H
#define MONOID_H

#include <functional>
#include <set>
#include <string>
#include <vector>

/* This is the base class for monoids over carrier set M. A monoid has a binary
 * associative operation '+' over M and a neutral element. Additionally, these
 * monoids are semi-lattice ordered, i.e. there exists a greatest lower bound
 * operator, defined such the greatest lower bound of the whole carrier set M
 * is the neutral element.
 *
 * Implementations for particular monoids can be found in monoids/. To define
 * a new monoid, we have to know the carrier set (M) and the binary associative
 * operator+ (F). Then we only have to write the specialization for 
 * neutral_element() and greatest_lower_bound(). If F is not specified, it is
 * by default std::plus<M>.
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
    Monoid() = default;
    Monoid(M value) : value(value) {}

    // Binary associative operator '+' over M calls Functor F.
    Monoid<M, F> &operator+=(Monoid<M,F> const &rhs) {
        value = F()(value, rhs.value);
        return *this;
    }

    // TODO uniform const T& syntax
    friend Monoid operator+(Monoid lhs, const Monoid &rhs) {
        lhs += rhs;
        return lhs;
    }

    friend bool operator<(Monoid const &l, Monoid const &r) {
        return l.value < r.value;
    }

    friend bool operator==(Monoid const &l, Monoid const &r) {
        return l.value == r.value;
    }

    // Greatest lower bound such that greatest_lower_bound(M) = '0'.
    static Monoid<M, F> greatest_lower_bound(
        std::vector<Monoid<M, F>> const &subset);

    // Returns the value associated with this monoid object.
    M get_value() const {
        return value;
    }

    // Neutral element '0' such that m + '0' = m for every m in M.
    // This is not constexpr, since M may be a non-literal type. See
    // http://en.cppreference.com/w/cpp/concept/LiteralType
    static Monoid<M, F> neutral_element();

    // Prints the value
    std::string to_string() const;

private:
    M value;
};

#include "monoids/variable_assignment.h"
#include "monoids/product.h"

#endif /* MONOID_H */

