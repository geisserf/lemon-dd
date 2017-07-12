#include "EvmddExpression.h"
#include <algorithm>
#include <iostream>

/*
 * Expression<float>
 */
template <>
std::string EvmddExpression<float>::toString() {
    return std::to_string(this->value);
}

template <>
bool EvmddExpression<float>::operator==(const EvmddExpression<float> &right) {
    return this->value == right.value;
}

template <>
bool EvmddExpression<float>::operator<(const EvmddExpression<float> &right) {
    return this->value < right.value;
}

template <>
bool EvmddExpression<float>::operator!=(const EvmddExpression<float> &right) {
    return this->value != right.value;
}

template <>
EvmddExpression<float> &EvmddExpression<float>::operator+(
    const EvmddExpression<float> &right) {
    NumericExpression *numericExpression = new EvmddExpression<float>();
    numericExpression->value = this->value + right.value;
    return *numericExpression;
}
template <>
EvmddExpression<float> &EvmddExpression<float>::operator-(
    const EvmddExpression<float> &right) {
    NumericExpression *numericExpression = new EvmddExpression<float>();
    numericExpression->value = this->value - right.value;
    return *numericExpression;
}

template <>
EvmddExpression<float> &EvmddExpression<float>::operator*(
    const EvmddExpression<float> &right) {
    NumericExpression *numericExpression = new EvmddExpression<float>();
    numericExpression->value = this->value * right.value;
    return *numericExpression;
}

template <>
EvmddExpression<float> &EvmddExpression<float>::operator/(
    const EvmddExpression<float> &right) {
    NumericExpression *numericExpression = new EvmddExpression<float>();
    numericExpression->value = this->value / right.value;
    return *numericExpression;
}

template <>
std::vector<NumericExpression> greates_lower_bound<NumericExpression>::
operator()(const NumericExpression &a,
           const std::vector<NumericExpression> &b) const {
    std::vector<EvmddExpression<float>> result;
    EvmddExpression<float> e;
    if (b.size() == 1) {
        if (b.front().value < a.value) {
            e.value = b.front().value;
        } else {
            e.value = a.value;
        }
    } else {
        e.value = a.value;
    }
    result.push_back(e);
    return result;
}

template <>
std::vector<NumericExpression> least_upper_bound<NumericExpression>::operator()(
    const NumericExpression &a, const std::vector<NumericExpression> &b) const {
    std::vector<EvmddExpression<float>> result;
    EvmddExpression<float> e;
    if (b.size() == 1) {
        if (b.front().value > a.value) {
            e.value = b.front().value;
        } else {
            e.value = a.value;
        }
    } else {
        e.value = a.value;
    }
    result.push_back(e);
    return result;
}

/*
 * Expression Variable Assignment
 */

template <>
std::string EvmddExpression<std::vector<VariableAssignment>>::toString() {
    std::string result = "{";
    for (size_t i = 0; i < this->value.size(); i++) {
        result = result + " " + value[i].toString();
    }
    return result + "}";
}

template <>
EvmddExpression<std::vector<VariableAssignment>>
    &EvmddExpression<std::vector<VariableAssignment>>::operator+(
        const EvmddExpression<std::vector<VariableAssignment>> &right) {
    EvmddExpression<std::vector<VariableAssignment>>
        *variableAssignmentExpression =
            new EvmddExpression<std::vector<VariableAssignment>>();
    variableAssignmentExpression->value.insert(
        variableAssignmentExpression->value.end(), this->value.begin(),
        this->value.end());
    variableAssignmentExpression->value.insert(
        variableAssignmentExpression->value.end(), right.value.begin(),
        right.value.end());
    return *variableAssignmentExpression;
}

template <>
EvmddExpression<std::vector<VariableAssignment>>
    &EvmddExpression<std::vector<VariableAssignment>>::operator-(
        const EvmddExpression<std::vector<VariableAssignment>> &right) {
    (void)right;
    throw std::logic_error("Not implemented");
}

template <>
EvmddExpression<std::vector<VariableAssignment>>
    &EvmddExpression<std::vector<VariableAssignment>>::operator*(
        const EvmddExpression<std::vector<VariableAssignment>> &right) {
    (void)right;
    throw std::logic_error("Not implemented");
}

template <>
EvmddExpression<std::vector<VariableAssignment>>
    &EvmddExpression<std::vector<VariableAssignment>>::operator/(
        const EvmddExpression<std::vector<VariableAssignment>> &right) {
    (void)right;
    throw std::logic_error("Not implemented");
}

template <>
bool EvmddExpression<std::vector<VariableAssignment>>::operator==(
    const EvmddExpression<std::vector<VariableAssignment>> &right) {
    return std::equal(this->value.begin(), this->value.end(),
                      right.value.begin());
}

template <>
bool EvmddExpression<std::vector<VariableAssignment>>::operator<(
    const EvmddExpression<std::vector<VariableAssignment>> &right) {
    // return
    // std::less(this->value.begin(),this->value.end(),right.value.begin());
    (void)right;
    return false;
}

template <>
bool EvmddExpression<std::vector<VariableAssignment>>::operator!=(
    const EvmddExpression<std::vector<VariableAssignment>> &right) {
    return !(this->value == right.value);
}

template <>
std::vector<VariableAssignementExpression>
greates_lower_bound<VariableAssignementExpression>::operator()(
    const VariableAssignementExpression &a,
    const std::vector<VariableAssignementExpression> &b) const {
    VariableAssignementExpression e;
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
    std::vector<VariableAssignementExpression> r;
    EvmddExpression<std::vector<VariableAssignment>> va_exp;
    va_exp.value = result;
    r.push_back(va_exp);
    return r;
}

template <>
std::vector<VariableAssignementExpression>
least_upper_bound<VariableAssignementExpression>::operator()(
    const VariableAssignementExpression &a,
    const std::vector<VariableAssignementExpression> &b) const {
    (void)a;
    (void)b;
    std::vector<VariableAssignementExpression> result;
    return result;
}

/*
 * Expression Tupel
 */

template <>
EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>> &
EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>::
operator+(const EvmddExpression<
          std::pair<VariableAssignementExpression, NumericExpression>> &right) {
    EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>
        *tupleExpression = new EvmddExpression<
            std::pair<VariableAssignementExpression, NumericExpression>>();
    tupleExpression->value.first = this->value.first + right.value.first;
    tupleExpression->value.second = this->value.second + right.value.second;
    return *tupleExpression;
}

template <>
EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>> &
EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>::
operator-(const EvmddExpression<
          std::pair<VariableAssignementExpression, NumericExpression>> &right) {
    EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>
        *tupleExpression = new EvmddExpression<
            std::pair<VariableAssignementExpression, NumericExpression>>();
    tupleExpression->value.first = this->value.first - right.value.first;
    tupleExpression->value.second = this->value.second - right.value.second;
    return *tupleExpression;
}

template <>
EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>
    &EvmddExpression<
        std::pair<VariableAssignementExpression, NumericExpression>>::operator
        *(const EvmddExpression<std::pair<VariableAssignementExpression,
                                          NumericExpression>> &right) {
    EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>
        *tupleExpression = new EvmddExpression<
            std::pair<VariableAssignementExpression, NumericExpression>>();
    tupleExpression->value.first = this->value.first * right.value.first;
    tupleExpression->value.second = this->value.second * right.value.second;
    return *tupleExpression;
}

template <>
EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>> &
EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>::
operator/(const EvmddExpression<
          std::pair<VariableAssignementExpression, NumericExpression>> &right) {
    EvmddExpression<std::pair<VariableAssignementExpression, NumericExpression>>
        *tupleExpression = new EvmddExpression<
            std::pair<VariableAssignementExpression, NumericExpression>>();
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
std::string EvmddExpression<
    std::pair<VariableAssignementExpression, NumericExpression>>::toString() {
    return "(" + this->value.first.toString() + ", " +
           this->value.second.toString() + ")";
}

template <>
bool EvmddExpression<TupleExpression>::operator<(
    const EvmddExpression<TupleExpression> &right) {
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

std::string VariableAssignment::toString() {
    return this->variable + "=" + std::to_string(this->value);
}

template class greates_lower_bound<NumericExpression>;
template class greates_lower_bound<VariableAssignementExpression>;
template class greates_lower_bound<TupleExpression>;
