#include "evmdd_expression.h"
#include <algorithm>
#include <cmath>
#include <iostream>

/*
 * Logic equals
*/

template <>
NumericExpression logic_equals<NumericExpression>::operator()(
    const NumericExpression &first, const NumericExpression &second) const {
    if (std::fabs(first.value - second.value) <=
        std::numeric_limits<float>::epsilon()) {
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
    if (std::fabs(first.value - 1) <= std::numeric_limits<float>::epsilon() ||
        std::fabs(second.value - 1) <= std::numeric_limits<float>::epsilon()) {
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
    throw std::logic_error(
        "VariableAssignmentExpression logic OR not supported");
}
/*
 * Logic AND
 */
template <>
NumericExpression logic_and<NumericExpression>::operator()(
    const NumericExpression &first, const NumericExpression &second) const {
    // float comparison to value ==1
    if (std::fabs(first.value - 1) <= std::numeric_limits<float>::epsilon() &&
        std::fabs(second.value - 1) <= std::numeric_limits<float>::epsilon()) {
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
    return std::to_string(value);
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
std::vector<NumericExpression> greates_lower_bound<NumericExpression>::
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
    VariableAssignmentExpression *variableAssignmentExpression =
        new VariableAssignmentExpression();
    variableAssignmentExpression->value.insert(
        variableAssignmentExpression->value.end(), this->value.begin(),
        this->value.end());
    variableAssignmentExpression->value.insert(
        variableAssignmentExpression->value.end(), right.value.begin(),
        right.value.end());
    return *variableAssignmentExpression;
}

template <>
VariableAssignmentExpression VariableAssignmentExpression::operator-(
    const VariableAssignmentExpression &right) const {
    (void)right;
    throw std::logic_error("Not implemented");
}

template <>
VariableAssignmentExpression VariableAssignmentExpression::operator*(
    const VariableAssignmentExpression &right) const {
    (void)right;
    throw std::logic_error("Not implemented");
}

template <>
VariableAssignmentExpression VariableAssignmentExpression::operator/(
    const VariableAssignmentExpression &right) const {
    (void)right;
    throw std::logic_error("Not implemented");
}

template <>
bool VariableAssignmentExpression::operator==(
    const VariableAssignmentExpression &right) const {
    return std::equal(this->value.begin(), this->value.end(),
                      right.value.begin());
}

template <>
bool VariableAssignmentExpression::operator<(
    const VariableAssignmentExpression &right) const {
    // return
    // std::less(this->value.begin(),this->value.end(),right.value.begin());
    (void)right;
    return false;
}

template <>
bool VariableAssignmentExpression::operator!=(
    const VariableAssignmentExpression &right) const {
    return !(this->value == right.value);
}

template <>
std::vector<VariableAssignmentExpression>
greates_lower_bound<VariableAssignmentExpression>::operator()(
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
    std::vector<VariableAssignmentExpression> result;
    return result;
}

/*
 * Expression Tupel
 */

template <>
TupleExpression TupleExpression::identity() {
    return TupleExpression{
        std::make_pair(VariableAssignmentExpression::identity(),
                       NumericExpression::identity())};
}

template <>
TupleExpression TupleExpression::operator+(const TupleExpression &right) const {
    TupleExpression *tupleExpression = new TupleExpression();
    tupleExpression->value.first = this->value.first + right.value.first;
    tupleExpression->value.second = this->value.second + right.value.second;
    return *tupleExpression;
}

template <>
TupleExpression TupleExpression::operator-(const TupleExpression &right) const {
    TupleExpression *tupleExpression = new TupleExpression();
    tupleExpression->value.first = this->value.first - right.value.first;
    tupleExpression->value.second = this->value.second - right.value.second;
    return *tupleExpression;
}

template <>
TupleExpression TupleExpression::operator*(
    const EvmddExpression<std::pair<VariableAssignmentExpression,
                                    NumericExpression>> &right) const {
    TupleExpression *tupleExpression = new TupleExpression();
    tupleExpression->value.first = this->value.first * right.value.first;
    tupleExpression->value.second = this->value.second * right.value.second;
    return *tupleExpression;
}

template <>
TupleExpression TupleExpression::operator/(const TupleExpression &right) const {
    TupleExpression *tupleExpression = new TupleExpression();
    tupleExpression->value.first = this->value.first / right.value.first;
    tupleExpression->value.second = this->value.second / right.value.second;
    return *tupleExpression;
}

template <>
std::vector<TupleExpression> greates_lower_bound<TupleExpression>::operator()(
    const TupleExpression &a, const std::vector<TupleExpression> &b) const {
    std::vector<TupleExpression> new_t;
    // bool add_this = false;
    bool added = false;
    if (b.size() > 0) {
        for (const TupleExpression tuple_in : b) {
            // std::cout<<"check if this.variable_assignments =
            // tuple_in.variable_assignments"<<std::endl;
            bool subset = tuple_in.value.first.value.size() > 0;

            for (VariableAssignment tuple_va : tuple_in.value.first.value) {
                bool found = false;
                for (VariableAssignment this_va : a.value.first.value) {
                    if (this_va.variable == tuple_va.variable &&
                        this_va.value == tuple_va.value) {
                        // std::cout<<this_va.variable<<"=="<<tuple_va.variable<<"&&"<<this_va.value<<"=="<<tuple_va.value<<std::endl;
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
                //  std::cout<<"check true"<<std::endl;
                TupleExpression n;
                n.value.first = a.value.first;
                n.value.second =
                    (a.value.second.value < tuple_in.value.second.value)
                        ? a.value.second
                        : tuple_in.value.second;
                // std::cout << "Add this to with lower value: " <<
                // n->toString()
                //          << std::endl;
                new_t.push_back(n);
                added = true;
            } else {
                // std::cout<<"check false"<<std::endl;
                TupleExpression n;
                n.value.first = tuple_in.value.first;
                n.value.second = tuple_in.value.second;
                new_t.push_back(n);
                // std::cout << "Add other to result : " << n->toString()
                //          << std::endl;
            }
        }
    }

    if (!added) {
        TupleExpression n;
        n.value.first = a.value.first;
        n.value.second = a.value.second;
        // std::cout << "Add this to result (not in in): " << n->toString()
        //          << std::endl;
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
    (void)right;
    return false;
}

/* Variable Assignment */
bool VariableAssignment::operator==(const VariableAssignment &rhs) const {
    return variable == rhs.variable && value == rhs.value;
}

bool VariableAssignment::operator!=(const VariableAssignment &rhs) const {
    return variable != rhs.variable || value != rhs.value;
}

bool VariableAssignment::operator<(const VariableAssignment &rhs) const {
    return this->variable < rhs.variable && this->value < rhs.value;
}

std::string VariableAssignment::toString() const {
    return this->variable + "=" + std::to_string(this->value);
}

template class least_upper_bound<NumericExpression>;
template class least_upper_bound<VariableAssignmentExpression>;
template class least_upper_bound<TupleExpression>;
template class greates_lower_bound<NumericExpression>;
template class greates_lower_bound<VariableAssignmentExpression>;
template class greates_lower_bound<TupleExpression>;
