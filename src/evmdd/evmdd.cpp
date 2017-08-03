#include "evmdd.h"
#include <algorithm>
#include <iterator>
#include <vector>

template <>
NumericExpression EvmddFactory<NumericExpression>::get_min(
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
EvmddFactory<VariableAssignmentExpression>::get_min(
    std::vector<Evmdd<VariableAssignmentExpression>> const &children) {
    // intersection
    VariableAssignmentExpression intersection;
    for (size_t i = 0; i < children.size(); ++i) {
        std::vector<VariableAssignment> tmp;
        // std::sort(children[i].input_value.value.begin(),
        //          children[i].input_value.value.end());

        std::set_intersection(
            children[i].input_value.value.begin(),
            children[i].input_value.value.end(), intersection.value.begin(),
            intersection.value.end(), std::back_inserter(tmp));

        intersection.value = tmp;
    }

    return intersection;
}
