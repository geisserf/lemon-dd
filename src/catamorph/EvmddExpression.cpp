#include "EvmddExpression.h"
#include <algorithm>
#include <iostream>

/*
 * Expression<float>
 */

template <>
bool EvmddExpression<float>::operator==(const EvmddExpression<float> &right) {
    return this->value == right.value;
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
std::vector<std::shared_ptr<NumericExpression>>
greates_lower_bound<NumericExpression>::operator()(
    const NumericExpression &a,
    const std::vector<std::shared_ptr<NumericExpression>> &b) const {
    std::vector<std::shared_ptr<EvmddExpression<float>>> result;
    auto e =
        std::shared_ptr<EvmddExpression<float>>(new EvmddExpression<float>());
    if (b.size() == 1) {
        if (b.front()->value < a.value) {
            e->value = b.front()->value;
        } else {
            e->value = a.value;
        }
    } else {
        e->value = a.value;
    }
    result.push_back(e);
    return result;
}

template <>
std::vector<std::shared_ptr<NumericExpression>>
least_upper_bound<NumericExpression>::operator()(
    const NumericExpression &a,
    const std::vector<std::shared_ptr<NumericExpression>> &b) const {
    std::vector<std::shared_ptr<EvmddExpression<float>>> result;
    auto e =
        std::shared_ptr<EvmddExpression<float>>(new EvmddExpression<float>());
    // std::cout<<"b.size:"<<b.size()<<"a.val:"<<a.value<<std::endl;
    if (b.size() == 1) {
        if (b.front()->value > a.value) {
            e->value = b.front()->value;
        } else {
            e->value = a.value;
        }
    } else {
        e->value = a.value;
    }
    // std::cout<<"   e.val "<<e->value<<std::endl;
    result.push_back(e);
    return result;
}

/*
 * Expression Variable Assignment
 */

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
bool EvmddExpression<std::vector<VariableAssignment>>::operator==(
    const EvmddExpression<std::vector<VariableAssignment>> &right) {
    return std::equal(this->value.begin(), this->value.end(),
                      right.value.begin());
}

template <>
bool EvmddExpression<std::vector<VariableAssignment>>::operator!=(
    const EvmddExpression<std::vector<VariableAssignment>> &right) {
    return !(this->value == right.value);
}

template <>
std::vector<std::shared_ptr<VariableAssignementExpression>>
greates_lower_bound<VariableAssignementExpression>::operator()(
    const VariableAssignementExpression &a,
    const std::vector<std::shared_ptr<VariableAssignementExpression>> &b)
    const {
    auto e = std::shared_ptr<VariableAssignementExpression>(
        new EvmddExpression<std::vector<VariableAssignment>>());
    // union
    std::vector<VariableAssignment> result;
    // add own variable assignments to new result
    for (VariableAssignment va : a.value) {
        VariableAssignment *a = new VariableAssignment();
        a->value = va.value;
        a->variable = va.variable;
        result.push_back(*a);
    }

    if (!b.empty()) {
        for (VariableAssignment va : b.front()->value) {
            if (std::find(result.begin(), result.end(), va) == result.end()) {
                VariableAssignment *nva = new VariableAssignment();
                nva->value = va.value;
                nva->variable = va.variable;
                result.push_back(*nva);
            }
        }
    }
    std::vector<std::shared_ptr<VariableAssignementExpression>> r;
    auto va_exp =
        std::shared_ptr<EvmddExpression<std::vector<VariableAssignment>>>(
            new EvmddExpression<std::vector<VariableAssignment>>());
    va_exp->value = result;
    r.push_back(va_exp);
    return r;
}

template <>
std::vector<std::shared_ptr<VariableAssignementExpression>>
least_upper_bound<VariableAssignementExpression>::operator()(
    const VariableAssignementExpression &a,
    const std::vector<std::shared_ptr<VariableAssignementExpression>> &b)
    const {
    (void)a;
    (void)b;
    std::vector<std::shared_ptr<VariableAssignementExpression>> result;
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
std::vector<std::shared_ptr<TupleExpression>>
greates_lower_bound<TupleExpression>::operator()(
    const TupleExpression &a,
    const std::vector<std::shared_ptr<TupleExpression>> &b) const {
    std::vector<std::shared_ptr<TupleExpression>> new_t;
    // bool add_this = false;
    bool added = false;
    if (b.size() > 0) {
        for (const std::shared_ptr<TupleExpression> tuple_in : b) {
            // std::cout<<"check if this.variable_assignments =
            // tuple_in.variable_assignments"<<std::endl;
            bool subset = tuple_in->value.first.value.size() > 0;

            for (VariableAssignment tuple_va : tuple_in->value.first.value) {
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

            if (subset || (tuple_in->value.first.value.size() == 0 &&
                           a.value.first.value.size() == 0)) {
                //  std::cout<<"check true"<<std::endl;
                std::shared_ptr<TupleExpression> n =
                    std::shared_ptr<TupleExpression>(
                        new EvmddExpression<std::pair<
                            VariableAssignementExpression, NumericExpression>>);
                n->value.first = a.value.first;
                n->value.second =
                    (a.value.second.value < tuple_in->value.second.value)
                        ? a.value.second
                        : tuple_in->value.second;
                // std::cout << "Add this to with lower value: " <<
                // n->toString()
                //          << std::endl;
                new_t.push_back(n);
                added = true;
            } else {
                // std::cout<<"check false"<<std::endl;
                std::shared_ptr<TupleExpression> n =
                    std::shared_ptr<TupleExpression>(
                        new EvmddExpression<std::pair<
                            VariableAssignementExpression, NumericExpression>>);
                n->value.first = tuple_in->value.first;
                n->value.second = tuple_in->value.second;
                new_t.push_back(n);
                // std::cout << "Add other to result : " << n->toString()
                //          << std::endl;
            }
        }
    }

    if (!added) {
        std::shared_ptr<TupleExpression> n = std::shared_ptr<TupleExpression>(
            new EvmddExpression<
                std::pair<VariableAssignementExpression, NumericExpression>>);
        n->value.first = a.value.first;
        n->value.second = a.value.second;
        // std::cout << "Add this to result (not in in): " << n->toString()
        //          << std::endl;
        new_t.push_back(n);
    }

    return new_t;
}
template <>
std::vector<std::shared_ptr<TupleExpression>>
least_upper_bound<TupleExpression>::operator()(
    const TupleExpression &a,
    const std::vector<std::shared_ptr<TupleExpression>> &b) const {
    (void)a;
    (void)b;
    std::vector<std::shared_ptr<TupleExpression>> result;
    return result;
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

template class greates_lower_bound<NumericExpression>;
template class greates_lower_bound<VariableAssignementExpression>;
template class greates_lower_bound<TupleExpression>;