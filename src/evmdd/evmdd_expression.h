#ifndef NUMERIC_CATAMORPH_EVMDDEXPRESSION_H
#define NUMERIC_CATAMORPH_EVMDDEXPRESSION_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

template <typename T>
class logic_equals {
public:
    T operator()(const T &first, const T &second) const;
};

template <typename T>
class logic_and {
public:
    T operator()(const T &first, const T &second) const;
};

template <typename T>
class logic_or {
public:
    T operator()(const T &first, const T &second) const;
};

class VariableAssignment {
public:
    std::string variable;
    int value;
    std::string toString() const;
    bool operator==(const VariableAssignment &rhs) const;
    bool operator!=(const VariableAssignment &rhs) const;
    bool operator<(const VariableAssignment &rhs) const;
};

template <typename T>
class EvmddExpression {
public:
    EvmddExpression() = default;
    EvmddExpression(T value) : value(value) {}
    T value;

    EvmddExpression<T> operator+(const EvmddExpression<T> &right) const;
    EvmddExpression<T> operator-(const EvmddExpression<T> &right) const;
    EvmddExpression<T> operator*(const EvmddExpression<T> &right) const;
    EvmddExpression<T> operator/(const EvmddExpression<T> &right) const;
    bool operator<(const EvmddExpression<T> &right) const;
    bool operator==(const EvmddExpression<T> &right) const;
    bool operator!=(const EvmddExpression<T> &right) const;
    std::string toString() const;

    // return identity element for type T
    static EvmddExpression<T> identity();
};

template <class T>
struct greates_lower_bound { // min
    std::vector<T> operator()(const T &a, const std::vector<T> &b) const;
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef std::vector<T> result_type;
};

template <class T>
struct least_upper_bound { // max
    std::vector<T> operator()(const T &a, const std::vector<T> &b) const;
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef std::vector<T> result_type;
};

using NumericExpression = EvmddExpression<float>;
using VariableAssignmentExpression =
    EvmddExpression<std::vector<VariableAssignment>>;
using TupleExpression =
    EvmddExpression<std::pair<VariableAssignmentExpression, NumericExpression>>;

#endif // NUMERIC_CATAMORPH_EVMDDEXPRESSION_H
