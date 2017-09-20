#ifndef MONOID_H
#define MONOID_H

#include <string>

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

    // Partial order on M, such that the greatest lower bound over M is '0'.
    friend bool less<M>(Monoid<M> const &l, Monoid<M> const &r);

    // Greatest lower bound such that greatest_lower_bound(M) = '0'.
    template <typename Container>
    M greatest_lower_bound(Container const &subset) const;

    // Returns the value associated with this monoid object.
    M get_value() const {
        return value;
    }

    // Neutral element '0' such that m + '0' = m for every m in M.
    static constexpr M neutral_element();

    // Prints the value
    std::string to_string() const;

private:
    M value;
};

#endif /* MONOID_H */
