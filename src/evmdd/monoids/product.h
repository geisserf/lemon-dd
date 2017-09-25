#include <algorithm>
#include <iterator>

// Cartesian Product of Monoids L and R with functions f and g as binary
// operator '+'
template <typename L, typename R, typename F, typename G>
using ProductMonoid = Monoid<std::pair<L, R>, std::pair<F, G>>;

// We have to fully specialize the class, since partial specializations for
// functions are not allowed
template <typename L, typename R, typename F, typename G>
class Monoid<std::pair<L, R>, std::pair<F, G>> {
public:
    Monoid() = default;
    Monoid(std::pair<L, R> value) : value(value) {}
    Monoid(Monoid<L, F> const &l, Monoid<R, G> const &r)
        : value({l.get_value(), r.get_value()}) {}

    // (l1,r1) + (l2,r2) = (F(l1,l2),G(r1,r2))
    ProductMonoid<L, R, F, G> &operator+=(
        ProductMonoid<L, R, F, G> const &rhs) {
        value.first = F()(value.first, rhs.value.first);
        value.second = G()(value.second, rhs.value.second);
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

    static ProductMonoid<L, R, F, G> greatest_lower_bound(
        std::vector<ProductMonoid<L, R, F, G>> const &subset);

    std::pair<L, R> get_value() const {
        return value;
    }

    static ProductMonoid<L, R, F, G> neutral_element() {
        return ProductMonoid<L, R, F, G>(Monoid<L, F>::neutral_element(),
                                         Monoid<R, G>::neutral_element());
    }

    std::string to_string() const {
        // This is a little overhead, but we do not have a method
        // std::pair<L,R>::to_string. In a perfect world we would delegate
        // printing in a monoid<L,F> to L and then we could call L.to_string()
        // here.
        Monoid<L, F> l(value.first);
        std::string result = l.to_string() + ",";
        Monoid<R, G> r(value.second);
        result += r.to_string();
        return result;
    }

private:
    std::pair<L, R> value;
};

template <typename L, typename R, typename F, typename G>
ProductMonoid<L, R, F, G> ProductMonoid<L, R, F, G>::greatest_lower_bound(
    std::vector<ProductMonoid<L, R, F, G>> const &subset) {
    // Maybe we should give greatest_lower_bound optionally a function which
    // retrieves the monoid of a container of monoid combinations. This would
    // allow us to use containers that contain other things than monoids (e.g.
    // monoid pairs) and use the function to retrieve the monoid we want.
    std::vector<Monoid<L, F>> l_monoids;
    std::transform(subset.begin(), subset.end(), std::back_inserter(l_monoids),
                   [](ProductMonoid<L, R, F, G> const &p) {
                       return Monoid<L, F>(p.get_value().first);
                   });
    Monoid<L, F> l_glb = Monoid<L>::greatest_lower_bound(l_monoids);
    std::vector<Monoid<R, G>> r_monoids;
    std::transform(subset.begin(), subset.end(), std::back_inserter(r_monoids),
                   [](ProductMonoid<L, R, F, G> const &p) {
                       return Monoid<R, G>(p.get_value().second);
                   });
    Monoid<R, G> r_glb = Monoid<R, G>::greatest_lower_bound(r_monoids);
    return {l_glb, r_glb};
}
