#ifndef NUMERIC_CATAMORPH_EVMDDEXPRESSION_H
#define NUMERIC_CATAMORPH_EVMDDEXPRESSION_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

class VariableAssignment {
public:
    std::string variable;
    int value;
    std::string toString();
    bool operator==(const VariableAssignment &rhs) const;
    bool operator!=(const VariableAssignment &rhs) const;
    bool operator<(const VariableAssignment &rhs) const;
};

template <typename T>
class EvmddExpression {
public:
    T value;

    EvmddExpression<T> &operator+(const EvmddExpression<T> &right);
    EvmddExpression<T> &operator-(const EvmddExpression<T> &right);
    EvmddExpression<T> &operator*(const EvmddExpression<T> &right);
    EvmddExpression<T> &operator/(const EvmddExpression<T> &right);
    bool operator<(const EvmddExpression<T> &right);
    bool operator==(const EvmddExpression<T> &right);
    bool operator!=(const EvmddExpression<T> &right);
    std::string toString();
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
using VariableAssignementExpression =
    EvmddExpression<std::vector<VariableAssignment>>;
using TupleExpression = EvmddExpression<
    std::pair<VariableAssignementExpression, NumericExpression>>;

#endif // NUMERIC_CATAMORPH_EVMDDEXPRESSION_H
