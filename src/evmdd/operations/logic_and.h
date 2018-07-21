#ifndef LOGIC_AND_H
#define LOGIC_AND_H

#include <functional>

// Template struct logic_and throws exception if parameters are
// none of the types int, float or double.
template <class T>
struct logic_and {
    static const T annihilator = 0, identity = 1;
    T operator()(const T &, const T &) const {
        throw std::logic_error("Non-boolean operands for logic_and");
    }
};

// Checks if the operands are binary
#ifndef DEBUG
template <class T>
void domain_check_and(const T &lhs, const T &rhs) {
    if (!((lhs == 0 || lhs == 1) && (rhs == 0 || rhs == 1))) {
        throw std::logic_error("Domain size >2 for logic_and");
    }
    assert((lhs == 0 || lhs == 1));
    assert((rhs == 0 || rhs == 1));
}
#endif

// Specialization for type int
template <>
struct logic_and<int> {
    int operator()(const int &lhs, const int &rhs) const {
#ifndef DEBUG
        domain_check_and(lhs, rhs);
#endif
        auto result = std::logical_and<int>()(lhs, rhs);
        return result;
    }
};

// Specialization for type double
template <>
struct logic_and<double> {
    double operator()(const double &lhs, const double &rhs) const {
#ifndef DEBUG
        domain_check_and(lhs, rhs);
#endif
        auto result = std::logical_and<double>()(lhs, rhs);
        return result;
    }
};

// Specialization for type float
template <>
struct logic_and<float> {
    float operator()(const float &lhs, const float &rhs) const {
#ifndef DEBUG
        domain_check_and(lhs, rhs);
#endif
        auto result = std::logical_and<float>()(lhs, rhs);
        return result;
    }
};

#endif /* LOGIC_AND_H */
