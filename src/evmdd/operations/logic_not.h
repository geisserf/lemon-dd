#ifndef LOGIC_NOT_H
#define LOGIC_NOT_H

#include <functional>

/**
 * Logical not operation disregards rhs and returns !lhs.
 * This is a hack to support unary evmdd operations via apply
 */

// Template class logic_or throws exception if parameters are
// none of the types int, float or double.
template <class T> struct logic_not {
  T operator()(const T &, const T &) const {
    throw std::logic_error("Non-boolean operand for logical_not");
  }
};

// Checks if the operand lhs is binary
#ifndef DEBUG
template <class T> void domain_check_not(const T &lhs) {
  if (!(lhs == 0 || lhs == 1)) {
    throw std::logic_error("Domain size >2 for logic_not");
  }
  assert((lhs == 0 || lhs == 1));
}
#endif

// Specialization for type int
template <> struct logic_not<int> {
  int operator()(const int &lhs, const int & /*rhs*/) const {
#ifndef DEBUG
    domain_check_not(lhs);
#endif
    auto result = std::logical_not<int>()(lhs);
    return result;
  }
};

// Specialization for type double
template <> struct logic_not<double> {
  double operator()(const double &lhs, const double & /*rhs*/) const {
#ifndef DEBUG
    domain_check_not(lhs);
#endif
    auto result = std::logical_not<double>()(lhs);
    return result;
  }
};

// Specialization for type float
template <> struct logic_not<float> {
  float operator()(const float &lhs, const float & /*rhs*/) const {
#ifndef DEBUG
    domain_check_not(lhs);
#endif
    auto result = std::logical_not<float>()(lhs);
    return result;
  }
};
#endif /* LOGIC_NOT_H */
