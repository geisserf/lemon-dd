#include "evmdd.h"
#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>

template <>
NumericExpression EvmddFactory<NumericExpression>::greatest_lower_bound(
    std::vector<Evmdd<NumericExpression>> const &children) {
    Evmdd<NumericExpression> min_weight_evmdd =
        *std::min_element(children.begin(), children.end(),
                          [](Evmdd<NumericExpression> const &e1,
                             Evmdd<NumericExpression> const &e2) {
                              return e1.input_value < e2.input_value;
                          });

    return min_weight_evmdd.input_value;
}

template <>
VariableAssignmentExpression
EvmddFactory<VariableAssignmentExpression>::greatest_lower_bound(
    std::vector<Evmdd<VariableAssignmentExpression>> const &children) {
    // intersection
    VariableAssignmentExpression intersection;
    for (size_t i = 0; i < children.size(); ++i) {
        std::vector<VariableAssignment> tmp;
        // TODO BUG set not sorted!!
        std::set_intersection(
            children[i].input_value.value.begin(),
            children[i].input_value.value.end(), intersection.value.begin(),
            intersection.value.end(), std::back_inserter(tmp));

        intersection.value = tmp;
    }

    return intersection;
}
template <>
TupleExpression EvmddFactory<TupleExpression>::greatest_lower_bound(
    std::vector<Evmdd<TupleExpression>> const &children) {
    NumericExpression min_weight = std::numeric_limits<float>::infinity();
    VariableAssignmentExpression intersection;

    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i].input_value.value.second < min_weight) {
            min_weight = children[i].input_value.value.second;
        }

        std::vector<VariableAssignment> tmp;
        // TODO BUG set not sorted!!
        std::set_intersection(children[i].input_value.value.first.value.begin(),
                              children[i].input_value.value.first.value.end(),
                              intersection.value.begin(),
                              intersection.value.end(),
                              std::back_inserter(tmp));

        intersection.value = tmp;
    }

    return TupleExpression({intersection, min_weight});
}
