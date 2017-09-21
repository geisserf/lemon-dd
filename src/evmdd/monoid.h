#ifndef MONOID_H
#define MONOID_H

#include <string>
#include <vector>
#include <set>

// A fact is a variable-value pair
using Fact = std::pair<std::string, int>;
using Facts = std::set<Fact>;

// Required for non-inlined friend definition of less,
// see https://stackoverflow.com/questions/4660123/
template <typename M>
class Monoid;

template <typename M>
bool less(Monoid<M> const &l, Monoid<M> const &r);

// Monoid over carrier set M
template <typename M>
class Monoid {
public:
    Monoid() = default;
    Monoid(M value) : value(value) {}

    // Binary associative operator '+' over M
    Monoid<M> &operator+=(Monoid<M> const &rhs) {
        value += rhs.get_value();
        return *this;
    }

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
    static Monoid<M> greatest_lower_bound(std::vector<Monoid<M>> const &subset);

    // Returns the value associated with this monoid object.
    M get_value() const {
        return value;
    }

    // Neutral element '0' such that m + '0' = m for every m in M.
    // This is not constexpr, since M may be a non-literal type. See
    // http://en.cppreference.com/w/cpp/concept/LiteralType
    static Monoid<M> neutral_element();

    // Prints the value
    std::string to_string() const;

private:
    M value;
};

#endif /* MONOID_H */
