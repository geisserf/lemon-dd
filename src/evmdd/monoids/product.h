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
    explicit Monoid(std::pair<L, R> value) : value(value) {}
    explicit Monoid(L l, R r) : value(std::make_pair(l,r)) {}
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

    // Monus operator '-'
    ProductMonoid<L, R, F, G> &operator-=(
        ProductMonoid<L, R, F, G> const &rhs) {
        // TODO The current concept requires us to create monoid objects for
        // <L,F> and <R,G> because otherwise we can't access their operator-.
        // There is probably a better concept.
        Monoid<L, F> l_monus =
            Monoid<L, F>{value.first} - Monoid<L, F>{rhs.value.first};
        Monoid<R, G> r_monus =
            Monoid<R, G>{value.second} - Monoid<R, G>{rhs.value.second};
        value.first = l_monus.get_value();
        value.second = r_monus.get_value();
        return *this;
    }

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

    static std::pair<L, R> greatest_lower_bound(std::pair<L, R> const &l,
                                                std::pair<L, R> const &r) {
        L l_glb = Monoid<L, F>::greatest_lower_bound(l.first, r.first);
        R r_glb = Monoid<R, G>::greatest_lower_bound(l.second, r.second);
        return std::make_pair(l_glb, r_glb);
    }

    std::pair<L, R> get_value() const {
        return value;
    }

    static std::pair<L, R> neutral_element() {
        return std::make_pair(Monoid<L, F>::neutral_element(),
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
