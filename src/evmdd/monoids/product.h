#include <algorithm>
#include <iterator>

// Cartesian Product of Monoids L and R with functions f and g as binary
// operator '+'
template <typename L, typename R, typename F = std::plus<L>,
          typename G = std::plus<R>>
using MonoidPair = std::pair<Monoid<L, F>, Monoid<R, G>>;

// Required for non-inlined friend definition of less,
// see https://stackoverflow.com/questions/4660123/
template <typename L, typename R, typename F, typename G>
class Monoid<MonoidPair<L, R, F, G>>;

template <typename L, typename R, typename F, typename G>
bool less(Monoid<MonoidPair<L, R, F, G>> const &l,
          Monoid<MonoidPair<L, R, F, G>> const &r);

// We have to fully specialize the class, since partial specializations for
// functions are not allowed
template <typename L, typename R, typename F, typename G>
class Monoid<MonoidPair<L, R, F, G>> {
public:
    Monoid() = default;
    Monoid(MonoidPair<L, R, F, G> value) : value(value) {}
    Monoid(Monoid<L, F> const &l, Monoid<R, G> const &r) : value({l, r}) {}

    // Binary associative operator '+' over M
    Monoid<MonoidPair<L, R, F, G>> &operator+=(
        Monoid<MonoidPair<L, R, F, G>> const &rhs) {
        value.first += rhs.value.first;
        value.second += rhs.value.second;
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

    static Monoid<MonoidPair<L, R, F, G>> greatest_lower_bound(
        std::vector<Monoid<MonoidPair<L, R, F, G>>> const &subset);

    MonoidPair<L, R, F, G> get_value() const {
        return value;
    }

    static Monoid<MonoidPair<L, R, F, G>> neutral_element() {
        return Monoid<MonoidPair<L, R, F, G>>(Monoid<L, F>::neutral_element(),
                                              Monoid<R, G>::neutral_element());
    }
    std::string to_string() const {
        return value.first.to_string() + "," + value.second.to_string();
    }

    // Convenience function to retrieve first element
    Monoid<L, F> const &first() const {
        return value.first;
    }

    // Convenience function to retrieve second element
    Monoid<R, G> const &second() const {
        return value.second;
    }

private:
    MonoidPair<L, R, F, G> value;
};

template <typename L, typename R, typename F = std::plus<L>,
          typename G = std::plus<R>>
using ProductMonoid = Monoid<std::pair<Monoid<L, F>, Monoid<R, G>>>;

template <typename L, typename R, typename F, typename G>
ProductMonoid<L, R, F, G> ProductMonoid<L, R, F, G>::greatest_lower_bound(
    std::vector<ProductMonoid<L, R, F, G>> const &subset) {
    // Maybe we should give greatest_lower_bound optionally a function which
    // retrieves the monoid of a container of monoid combinations. This would
    // allow us to use containers that contain other things than monoids (e.g.
    // monoid pairs) and use the function to retrieve the monoid we want.
    std::vector<Monoid<L, F>> l_monoids;
    std::transform(
        subset.begin(), subset.end(), std::back_inserter(l_monoids),
        [](ProductMonoid<L, R, F, G> const &p) { return p.first(); });
    Monoid<L, F> l_glb = Monoid<L>::greatest_lower_bound(l_monoids);
    std::vector<Monoid<R, G>> r_monoids;
    std::transform(
        subset.begin(), subset.end(), std::back_inserter(r_monoids),
        [](ProductMonoid<L, R, F, G> const &p) { return p.second(); });
    Monoid<R, G> r_glb = Monoid<R, G>::greatest_lower_bound(r_monoids);
    return {l_glb, r_glb};
}
