#ifndef LOGIC_OR_H
#define LOGIC_OR_H

#include <functional>

// Template struct logic_or throws exception if parameters are
// none of the types int, float or double.
template <class T>
struct logic_or {
    T operator()(const T &, const T &) const {
        throw std::logic_error("Non-boolean operand for logical_or");
    }
};

// Checks if the operands are binary
#ifndef DEBUG
template <class T>
void domain_check_or(const T &lhs, const T &rhs) {
    if (!((lhs == 0 || lhs == 1) && (rhs == 0 || rhs == 1))) {
        throw std::logic_error("Domain size >2 for logic_or");
    }
    assert((lhs == 0 || lhs == 1));
    assert((rhs == 0 || rhs == 1));
}
#endif

// Specialization for type int
template <>
struct logic_or<int> {
    logic_or() : annihilator(1), identity(0) {}
    int operator()(const int &lhs, const int &rhs) const {
#ifndef DEBUG
        domain_check_or(lhs, rhs);
#endif
        auto result = std::logical_or<int>()(lhs, rhs);
        return result;
    }
    int annihilator, identity;
    constexpr bool has_identity() const {
        return true;
    }

    constexpr bool has_annihilator() const {
        return true;
    }
};

// Specialization for type double
template <>
struct logic_or<double> {
    logic_or() : annihilator(1), identity(0) {}
    double operator()(const double &lhs, const double &rhs) const {
#ifndef DEBUG
        domain_check_or(lhs, rhs);
#endif
        auto result = std::logical_or<double>()(lhs, rhs);
        return result;
    }
    double annihilator, identity;
    constexpr bool has_identity() const {
        return true;
    }

    constexpr bool has_annihilator() const {
        return true;
    }
};

// Specialization for type float
template <>
struct logic_or<float> {
    logic_or() : annihilator(1), identity(0) {}
    float operator()(const float &lhs, const float &rhs) const {
#ifndef DEBUG
        domain_check_or(lhs, rhs);
#endif
        auto result = std::logical_or<float>()(lhs, rhs);
        return result;
    }
    float annihilator, identity;
    constexpr bool has_identity() const {
        return true;
    }

    constexpr bool has_annihilator() const {
        return true;
    }
};

#endif /* LOGIC_NOT_H */
