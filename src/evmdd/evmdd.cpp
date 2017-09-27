// #include "evmdd.h"
// #include <algorithm>
// #include <iterator>
// #include <limits>
// #include <vector>
// 
// template <>
// NumericExpression EvmddFactory<NumericExpression>::greatest_lower_bound(
//     std::vector<Evmdd<NumericExpression>> const &children) {
//     Evmdd<NumericExpression> min_weight_evmdd =
//         *std::min_element(children.begin(), children.end(),
//                           [](Evmdd<NumericExpression> const &e1,
//                              Evmdd<NumericExpression> const &e2) {
//                               return e1.input_value < e2.input_value;
//                           });
// 
//     return min_weight_evmdd.input_value;
// }
// 
// template <>
// VariableAssignmentExpression
// EvmddFactory<VariableAssignmentExpression>::greatest_lower_bound(
//     std::vector<Evmdd<VariableAssignmentExpression>> const &children) {
//     // intersection
//     VariableAssignmentExpression intersection;
//     intersection.value = children[0].input_value.value;
// 
//     for (size_t i = 1; i < children.size(); ++i) {
//         std::vector<VariableAssignment> tmp;
//         auto tmp_child = children[i].input_value.value;
// 
//         for (VariableAssignment va : intersection.value) {
//             for (VariableAssignment c_va : children[i].input_value.value) {
//                 if (va == c_va) {
//                     tmp.push_back(va);
// 
//                     // std::cout<< "interseciton between " << va.toString()<< "
//                     // and "<<c_va.toString()<<std::endl;
//                 }
//             }
// 
//             // if (std::find(tmp_child.begin(), tmp_child.end(), va) !=
//             //    tmp_child.end()) {
//             //    tmp.push_back(va);
//             //}
//         }
//         // std::cout<<"new intersection"<<std::endl;
//         intersection.value = tmp;
//     }
// 
//     return intersection;
// }
// 
// template <>
// TupleExpression EvmddFactory<TupleExpression>::greatest_lower_bound(
//     std::vector<Evmdd<TupleExpression>> const &children) {
//     NumericExpression min_weight;
//     min_weight.value = children[0].input_value.value.second.value;
//     VariableAssignmentExpression intersection;
//     intersection.value = children[0].input_value.value.first.value;
//     for (size_t i = 1; i < children.size(); ++i) {
//         if (children[i].input_value.value.second.value < min_weight.value) {
//             min_weight.value = children[i].input_value.value.second.value;
//         }
// 
//         std::vector<VariableAssignment> tmp;
//         auto tmp_child = children[i].input_value.value.first.value;
// 
//         for (VariableAssignment va : intersection.value) {
//             if (std::find(tmp_child.begin(), tmp_child.end(), va) !=
//                 tmp_child.end()) {
//                 tmp.push_back(va);
//             }
//         }
// 
//         intersection.value = tmp;
//     }
// 
//     return TupleExpression({intersection, min_weight});
// }
