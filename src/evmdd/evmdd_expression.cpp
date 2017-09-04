#include "evmdd_expression.h"
#include "../utils/math_utils.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

/*
 * Logic not
 */

template <>
NumericExpression logic_not<NumericExpression>::operator()(
    const NumericExpression &first, const NumericExpression &second) const {
    assert(first.value == second.value);
    (void)second;
    if (MathUtils::is_equal(first.value, 1)) {
        return NumericExpression{0.0f};
    }

    if (MathUtils::is_equal(first.value, 0)) {
        return NumericExpression{1.0f};
    }

    throw std::logic_error("NOT operator applied to non boolean value" +
                           std::to_string(first.value));
}

template <>
VariableAssignmentExpression logic_not<VariableAssignmentExpression>::
operator()(const VariableAssignmentExpression &first,
           const VariableAssignmentExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error(
        "VariableAssignmentExpression logic NOT not supported");
}

template <>
TupleExpression logic_not<TupleExpression>::operator()(
    const TupleExpression &first, const TupleExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error("TupleExpression logic NOT not supported");
}

/*
 * Logic equals
*/

template <>
NumericExpression logic_equals<NumericExpression>::operator()(
    const NumericExpression &first, const NumericExpression &second) const {
    if (MathUtils::is_equal(first.value, second.value)) {
        return NumericExpression{1.0f};
    } else {
        return NumericExpression{0.0f};
    }
}

template <>
VariableAssignmentExpression logic_equals<VariableAssignmentExpression>::
operator()(const VariableAssignmentExpression &first,
           const VariableAssignmentExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error(
        "Logic EQUAL not implemented for variableAssignmentExpression");
}

template <>
TupleExpression logic_equals<TupleExpression>::operator()(
    const TupleExpression &first, const TupleExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error("Logic EQUALS not implemented for TupleExpression");
}

/*
 * Logic OR
 */
template <>
NumericExpression logic_or<NumericExpression>::operator()(
    const NumericExpression &first, const NumericExpression &second) const {
    // float comparison to value ==1
    if (MathUtils::is_equal(first.value, 1) ||
        MathUtils::is_equal(second.value, 1)) {
        return NumericExpression{1.0f};
    }

    return NumericExpression{0.0f};
}

template <>
VariableAssignmentExpression logic_or<VariableAssignmentExpression>::operator()(
    const VariableAssignmentExpression &first,
    const VariableAssignmentExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error(
        "VariableAssignmentExpression logic OR not supported");
}

template <>
TupleExpression logic_or<TupleExpression>::operator()(
    const TupleExpression &first, const TupleExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error("TupleExpression logic OR not supported");
}
/*
 * Logic AND
 */
template <>
NumericExpression logic_and<NumericExpression>::operator()(
    const NumericExpression &first, const NumericExpression &second) const {
    // float comparison to value ==1
    if (MathUtils::is_equal(first.value, 1) &&
        MathUtils::is_equal(second.value, 1)) {
        return NumericExpression{1.0f};
    }

    return NumericExpression{0.0f};
}

template <>
VariableAssignmentExpression logic_and<VariableAssignmentExpression>::
operator()(const VariableAssignmentExpression &first,
           const VariableAssignmentExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error(
        "VariableAssignmentExpression logic AN not supported");
}

template <>
TupleExpression logic_and<TupleExpression>::operator()(
    const TupleExpression &first, const TupleExpression &second) const {
    (void)first;
    (void)second;
    throw std::logic_error(
        "VariableAssignmentExpression logic AND not supported");
}

/*
 * Expression<float>
 */

template <>
NumericExpression NumericExpression::identity() {
    return NumericExpression{0.0f};
}

template <>
std::string NumericExpression::toString() const {
    // We use the stream operator to print e instead of std::to_string,
    // because then we can set the precision before we print (otherwise
    // even integer constant n is printed as n.000000
    std::ostringstream out;
    out << value;
    return out.str();
}

template <>
bool NumericExpression::operator==(const NumericExpression &right) const {
    return value == right.value;
}

template <>
bool NumericExpression::operator<(const NumericExpression &right) const {
    return value < right.value;
}

template <>
bool NumericExpression::operator!=(const NumericExpression &right) const {
    return value != right.value;
}

template <>
NumericExpression NumericExpression::operator+(
    const NumericExpression &right) const {
    return NumericExpression{value + right.value};
}

template <>
NumericExpression NumericExpression::operator-(
    const NumericExpression &right) const {
    return NumericExpression{value - right.value};
}

template <>
NumericExpression NumericExpression::operator*(
    const NumericExpression &right) const {
    return NumericExpression{value * right.value};
}

template <>
NumericExpression NumericExpression::operator/(
    const NumericExpression &right) const {
    return NumericExpression{value / right.value};
}

template <>
std::vector<NumericExpression> greatest_lower_bound<NumericExpression>::
operator()(const NumericExpression &a,
           const std::vector<NumericExpression> &b) const {
    std::vector<NumericExpression> result;
    NumericExpression e = a;

    for (NumericExpression b_ : b) {
        if (b_ < e) {
            e = b_;
        }
    }
    result.push_back(e);
    return result;
}

template <>
std::vector<NumericExpression> least_upper_bound<NumericExpression>::operator()(
    const NumericExpression &a, const std::vector<NumericExpression> &b) const {
    std::vector<NumericExpression> result;
    NumericExpression e = a;
    for (NumericExpression b_ : b) {
        if (b_.value > e.value) {
            e = b_;
        }
    }

    result.push_back(e);
    return result;
}

/*
 * Expression Variable Assignment
 */
template <>
VariableAssignmentExpression VariableAssignmentExpression::identity() {
    return VariableAssignmentExpression{{}};
}

template <>
std::string VariableAssignmentExpression::toString() const {
    std::string result = "{";
    for (size_t i = 0; i < value.size(); i++) {
        result = result + " " + value[i].toString();
    }
    return result + "}";
}

template <>
VariableAssignmentExpression VariableAssignmentExpression::operator+(
    const VariableAssignmentExpression &right) const {
    VariableAssignmentExpression result;
    result.value.insert(result.value.end(), value.begin(), value.end());
    result.value.insert(result.value.end(), right.value.begin(),
                        right.value.end());
    return result;
}

template <>
VariableAssignmentExpression VariableAssignmentExpression::operator-(
    const VariableAssignmentExpression &right) const {
    VariableAssignmentExpression res;
    for (VariableAssignment const &va : value) {
        if (std::find(right.value.begin(), right.value.end(), va) ==
            right.value.end()) {
            res.value.push_back(va);
        }
    }

    return res;
}

template <>
VariableAssignmentExpression VariableAssignmentExpression::operator*(
    const VariableAssignmentExpression &right) const {
    (void)right;
    throw std::logic_error(" VAE \"*\"Not implemented");
}

template <>
VariableAssignmentExpression VariableAssignmentExpression::operator/(
    const VariableAssignmentExpression &right) const {
    (void)right;
    throw std::logic_error("VAE \"/\"  Not implemented");
}

template <>
bool VariableAssignmentExpression::operator==(
    const VariableAssignmentExpression &right) const {
    if (value.size() != right.value.size()) {
        return false;
    }

    for (size_t i = 0; i < value.size(); ++i) {
        if (!(value[i] != right.value[i])) {
            return false;
        }
    }

    return true;

    // return std::equal(value.begin(), value.end(), right.value.begin());
}

template <>
bool VariableAssignmentExpression::operator<(
    const VariableAssignmentExpression &right) const {
    if (value.size() != right.value.size()) {
        return value.size() < right.value.size();
    }

    for (size_t i = 0; i < value.size(); ++i) {
        if (!(value[i] < right.value[i])) {
            return false;
        }
    }

    /*
        for (VariableAssignment const &va : value) {
            if (std::find(right.value.begin(), right.value.end(), va) ==
                right.value.end()) {
                return false;
            }
        }
    */
    return true;
}

template <>
bool VariableAssignmentExpression::operator!=(
    const VariableAssignmentExpression &right) const {
    if (value.size() != right.value.size()) {
        return true;
    }

    for (size_t i = 0; i < value.size(); ++i) {
        if (!(value[i] != right.value[i])) {
            return true;
        }
    }

    return false;
}

template <>
std::vector<VariableAssignmentExpression>
greatest_lower_bound<VariableAssignmentExpression>::operator()(
    const VariableAssignmentExpression &a,
    const std::vector<VariableAssignmentExpression> &b) const {
    VariableAssignmentExpression e;
    // union
    std::vector<VariableAssignment> result;
    // add own variable assignments to new result
    for (VariableAssignment va : a.value) {
        VariableAssignment a;
        a.value = va.value;
        a.variable = va.variable;
        result.push_back(a);
    }

    if (!b.empty()) {
        for (VariableAssignment va : b.front().value) {
            if (std::find(result.begin(), result.end(), va) == result.end()) {
                VariableAssignment nva;
                nva.value = va.value;
                nva.variable = va.variable;
                result.push_back(nva);
            }
        }
    }
    std::vector<VariableAssignmentExpression> r;
    VariableAssignmentExpression va_exp;
    va_exp.value = result;
    r.push_back(va_exp);
    return r;
}

template <>
std::vector<VariableAssignmentExpression>
least_upper_bound<VariableAssignmentExpression>::operator()(
    const VariableAssignmentExpression &a,
    const std::vector<VariableAssignmentExpression> &b) const {
    (void)a;
    (void)b;
    throw std::logic_error("Least upper bound VAE Not Implemented");
    std::vector<VariableAssignmentExpression> result;
    return result;
}

/*
 * Expression Tuple
 */

template <>
TupleExpression TupleExpression::identity() {
    return TupleExpression{
        std::make_pair(VariableAssignmentExpression::identity(),
                       NumericExpression::identity())};
}

template <>
TupleExpression TupleExpression::operator+(const TupleExpression &right) const {
    TupleExpression tupleExpression;
    tupleExpression.value.first = value.first + right.value.first;
    tupleExpression.value.second = value.second + right.value.second;
    return tupleExpression;
}

template <>
TupleExpression TupleExpression::operator-(const TupleExpression &right) const {
    TupleExpression tupleExpression;
    tupleExpression.value.first = value.first - right.value.first;
    tupleExpression.value.second = value.second - right.value.second;
    return tupleExpression;
}

template <>
TupleExpression TupleExpression::operator*(
    const EvmddExpression<std::pair<VariableAssignmentExpression,
                                    NumericExpression>> &right) const {
    TupleExpression tupleExpression;
    tupleExpression.value.first = value.first * right.value.first;
    tupleExpression.value.second = value.second * right.value.second;
    return tupleExpression;
}

template <>
TupleExpression TupleExpression::operator/(const TupleExpression &right) const {
    TupleExpression tupleExpression;
    tupleExpression.value.first = value.first / right.value.first;
    tupleExpression.value.second = value.second / right.value.second;
    return tupleExpression;
}

template <>
std::vector<TupleExpression> greatest_lower_bound<TupleExpression>::operator()(
    const TupleExpression &a, const std::vector<TupleExpression> &b) const {
    std::vector<TupleExpression> new_t;
    // bool add_this = false;
    bool added = false;
    if (b.size() > 0) {
        for (const TupleExpression tuple_in : b) {
            bool subset = tuple_in.value.first.value.size() > 0;

            for (VariableAssignment tuple_va : tuple_in.value.first.value) {
                bool found = false;
                for (VariableAssignment this_va : a.value.first.value) {
                    if (this_va.variable == tuple_va.variable &&
                        this_va.value == tuple_va.value) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    subset = false;
                    break;
                }
            }

            if (subset || (tuple_in.value.first.value.size() == 0 &&
                           a.value.first.value.size() == 0)) {
                TupleExpression n;
                n.value.first = a.value.first;
                n.value.second =
                    (a.value.second.value < tuple_in.value.second.value)
                        ? a.value.second
                        : tuple_in.value.second;
                new_t.push_back(n);
                added = true;
            } else {
                TupleExpression n;
                n.value.first = tuple_in.value.first;
                n.value.second = tuple_in.value.second;
                new_t.push_back(n);
            }
        }
    }

    if (!added) {
        TupleExpression n;
        n.value.first = a.value.first;
        n.value.second = a.value.second;
        new_t.push_back(n);
    }

    return new_t;
}
template <>
std::vector<TupleExpression> least_upper_bound<TupleExpression>::operator()(
    const TupleExpression &a, const std::vector<TupleExpression> &b) const {
    (void)a;
    (void)b;
    std::vector<TupleExpression> result;
    throw std::logic_error("LUB TupleExpression Not implemented");
    return result;
}

template <>
std::string TupleExpression::toString() const {
    return "(" + value.first.toString() + ", " + value.second.toString() + ")";
}

template <>
bool TupleExpression::operator<(const TupleExpression &right) const {
    // return
    // std::less(this->value.begin(),this->value.end(),right.value.begin());

    return value.first < right.value.first && value.second < right.value.second;

    //    return false;
}

/* Variable Assignment */
bool VariableAssignment::operator==(const VariableAssignment &rhs) const {
    return variable == rhs.variable && value == rhs.value;
}

bool VariableAssignment::operator!=(const VariableAssignment &rhs) const {
    return variable != rhs.variable || value != rhs.value;
}

bool VariableAssignment::operator<(const VariableAssignment &rhs) const {
    if (variable < rhs.variable) {
        return true;
    } else if (variable == rhs.variable) {
        return value < rhs.value;
    }
    return false;
}

std::string VariableAssignment::toString() const {
    return this->variable + "=" + std::to_string(this->value);
}

template class least_upper_bound<NumericExpression>;
template class least_upper_bound<VariableAssignmentExpression>;
template class least_upper_bound<TupleExpression>;
template class greatest_lower_bound<NumericExpression>;
template class greatest_lower_bound<VariableAssignmentExpression>;
template class greatest_lower_bound<TupleExpression>;
