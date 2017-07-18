#include "evmdd.h"
#include "evmdd_expression.h"

using std::vector;

// template <typename T>
// std::vector<T> Evmdd<T>::get_min() const {
//     return evaluate(std::map<std::string, std::vector<int>>(),
//                     greates_lower_bound<T>());
// }
//
// template <typename T>
// std::vector<T> Evmdd<T>::get_max() const {
//     return evaluate(std::map<std::string, std::vector<int>>(),
//                     least_upper_bound<T>());
// }
//
// template <typename T>
// std::vector<T> Evmdd<T>::evaluate_partial(
//     std::map<std::string, std::vector<int>> const &state) const {
//     return evaluate(state, greates_lower_bound<T>());
// }
//
// template <typename T>
// template <typename EvaluationFunction>
// std::vector<T> Evmdd<T>::evaluate(
//     std::map<std::string, std::vector<int>> const &state,
//     EvaluationFunction evaluationFunction) const {
//     std::map<int, std::vector<T>> partialEvaluations;
//
//     std::queue<Edge<T>> openList;
//     openList.push(evmdd);
//     Edge<T> currentEdge;
//     while (openList.size() > 0) {
//         currentEdge = openList.front();
//         openList.pop();
//         auto p = partialEvaluations.find(currentEdge.successor.id);
//         auto res = calculate_partial_evaluation(currentEdge,
//         partialEvaluations,
//                                                 evaluationFunction);
//         if (p == partialEvaluations.end()) {
//             partialEvaluations.insert(
//                 std::pair<int, std::vector<T>>(currentEdge.successor.id,
//                 res));
//         } else {
//             p->second = res;
//         }
//
//         if (!currentEdge.successor.outgoing.empty()) {
//             if (state.find(currentEdge.successor.variable) != state.end()) {
//                 // variable is in state,  add successor outgoing edges to
//                 open
//                 // list
//                 auto const &state_values =
//                     state.at(currentEdge.successor.variable);
//                 for (const auto &state_value : state_values) {
//                     if (currentEdge.successor.outgoing.size() >
//                             (unsigned)state_value &&
//                         state_value >= 0) {
//                         for (auto e : currentEdge.successor.outgoing) {
//                             if (e.value == (unsigned)state_value) {
//                                 openList.push(e);
//                             }
//                         }
//
//                     } else {
//                         std::cerr << "Error Evaluating EVMDD: "
//                                   << currentEdge.successor.variable
//                                   << " with value: " << state_value
//                                   << std::endl;
//                     }
//                 }
//             } else {
//                 // variable not in state
//                 // so we assume that variable can take on all values
//                 for (Edge<T> &succ_edge : currentEdge.successor.outgoing) {
//                     openList.push(succ_edge);
//                 }
//             }
//         }
//     }
//     return partialEvaluations.find(0)->second;
// }
//
// template <typename T>
// template <typename EvaluationFunction>
// std::vector<T> Evmdd<T>::calculate_partial_evaluation(
//     Edge<T> const &incoming_edge,
//     std::map<int, std::vector<T>> const &partialEvaluations,
//     EvaluationFunction evaluationFunction) const {
//     if (incoming_edge.predecessor.variable == "") {
//         T current = incoming_edge.label.expression;
//         std::vector<T> result;
//         T e;
//         e.value = current.value;
//         result.push_back(e);
//         return result;
//     } else {
//         auto pe = partialEvaluations.find(incoming_edge.predecessor.id);
//
//         std::vector<T> predecessor_partial_evaluations;
//         if (pe != partialEvaluations.end()) {
//             predecessor_partial_evaluations = pe->second;
//         }
//
//         std::vector<T> result;
//         auto current_pe =
//         partialEvaluations.find(incoming_edge.successor.id);
//         if (current_pe != partialEvaluations.end()) {
//             result = current_pe->second;
//         }
//
//         for (auto &it : predecessor_partial_evaluations) {
//             T current = it + incoming_edge.label.expression;
//             auto x = evaluationFunction(current, result);
//             result = x;
//         }
//         return result;
//     }
// }
//

template <typename T>
Evmdd<T>::Evmdd(T input, Node<T> const &entry_node)
    : input_value(input), entry_node(entry_node) {}

template <typename T>
Evmdd<T> EvmddFactory<T>::make_const_evmdd(T weight) {
    return Evmdd<T>(weight, node_factory.get_terminal_node());
}

template <>
Evmdd<NumericExpression> EvmddFactory<NumericExpression>::make_var_evmdd(
    int level, std::string const &var, int domain) {
    vector<Edge<NumericExpression>> children;
    for (int i = 0; i < domain; ++i) {
        children.emplace_back(Label<NumericExpression>(i), 0);
    }
    Node<NumericExpression> node = node_factory.make_node(level, var, children);
    return Evmdd<NumericExpression>(0, node);
}

template <typename T>
Evmdd<T> EvmddFactory<T>::make_var_evmdd(int level, std::string const &var,
                                         int domain) {
    (void) level;
    (void) var;
    (void) domain;
    throw(std::logic_error("Make Var EVMDD not implemented"));
}

// template <typename T>
// Evmdd<T> Evmdd<T>::apply(Evmdd<T> const &other,
//                          expression_r<Evmdd<T>> const &oper) {
//     this->evmdd = _apply(this->evmdd, other.evmdd, oper);
//
//     return *this;
// }
//
// template <typename T>
// Edge<T> Evmdd<T>::_apply(Edge<T> const &first, Edge<T> const &other,
//                          expression_r<Evmdd<T>> const &oper) {
//     // terminal case
//     if (first.successor.id == 0 && other.successor.id == 0) {
//         return _terminal_value(first, other, oper);
//     }
//     // get the higher level
//     int level = std::max(first.successor.level, other.successor.level);
//     // align edges
//     auto self_children = _align_levels(first, other);
//     auto other_children = _align_levels(other, first);
//     std::vector<Edge<T>> children;
//     // new children
//     for (size_t c = 0; c < self_children.size(); c++) {
//         // apply operator on edges of both children
//         children.push_back(
//             apply_operator(self_children[c], other_children[c], oper));
//     }
//
//     // get minimum of child edge labels
//     auto rw = std::min_element(
//         children.begin(), children.end(), [](Edge<T> e1, Edge<T> e2) {
//             return e1.label.expression < e2.label.expression;
//         });
//
//     std::string var = "";
//     if (first.successor.level == level) {
//         var = first.successor.variable;
//     } else {
//         var = other.successor.variable;
//     }
//
//     Node<T> result_succ = Node<T>::makeNode(children, var, level);
//
//     T result_weight = rw->label.expression;
//     // subtract min from children (moved back up)
//
//     for (auto it = result_succ.outgoing.begin();
//          it != result_succ.outgoing.end(); it++) {
//         it->label.expression = it->label.expression - result_weight;
//     }
//
//     Edge<T> res(result_succ);
//
//     res.label.expression = result_weight;
//     res.value = first.value;
//
//     return res;
// }
//
// template <typename T>
// Edge<T> Evmdd<T>::apply_operator(Edge<T> edge1, Edge<T> edge2,
//                                  expression_r<Evmdd<T>> const &oper) {
//     return _apply(edge1, edge2, oper);
// }
//
// template <typename T>
// std::vector<Edge<T>> Evmdd<T>::_align_levels(Edge<T> edge1, Edge<T> edge2) {
//     std::vector<Edge<T>> res;
//     if (edge1.successor.level >= edge2.successor.level) {
//         for (Edge<T> child : edge1.successor.outgoing) {
//             T newexp = child.label.expression + edge1.label.expression;
//             Edge<T> ne = Edge<T>(child);
//             ne.label.expression = newexp;
//             res.push_back(ne);
//         }
//     } else {
//         int value = 0;
//         for (Edge<T> child : edge2.successor.outgoing) {
//             Edge<T> ne = Edge<T>(edge1);
//             ne.value = value;
//             value++;
//             res.push_back(ne);
//         }
//     }
//
//     return res;
// }
//
// template <>
// Edge<NumericExpression> Evmdd<NumericExpression>::makeVarEvmdd(
//     const std::string var, unsigned int domain, int level) {
//     // terminal node
//     Node<NumericExpression> terminal =
//         Node<NumericExpression>::makeTerminalNode();
//
//     // Incoming
//     Label<NumericExpression> incoming_label;
//     NumericExpression expression;
//     expression.value = 0;
//     incoming_label.expression = expression;
//
//     // Variable Node
//
//     // for each domain value add edge from incoming_edge.get()->successor to
//     // terminal with weight = domain value
//     std::vector<Edge<NumericExpression>> outgoing;
//     for (unsigned int d = 0; d < domain; d++) {
//         Label<NumericExpression> edge_label;
//         NumericExpression edge_expression;
//         edge_expression.value = d;
//         edge_label.expression = edge_expression;
//         Edge<NumericExpression> edge;
//         edge.label = edge_label;
//         edge.value = d;
//         outgoing.push_back(edge);
//     }
//
//     Node<NumericExpression> variable_node =
//         Node<NumericExpression>::makeNode(outgoing, var, level);
//
//     Edge<NumericExpression> incoming_edge(variable_node);
//     incoming_edge.label = incoming_label;
//     incoming_edge.value = 0;
//     variable_node.incoming.push_back(incoming_edge);
//     return incoming_edge;
// }
//
// template <typename T>
// Edge<T> Evmdd<T>::makeConstEvmdd(T weight) {
//     return makeConstEvmdd(weight, 0);
// }
//
// template <typename T>
// Edge<T> Evmdd<T>::makeConstEvmdd(T weight, unsigned int edge_value) {
//     Edge<T> edge;
//     Label<T> l;
//     l.expression = weight;
//     edge.label = l;
//     edge.value = edge_value;
//     return edge;
// }

template class Evmdd<NumericExpression>;
template class Evmdd<VariableAssignementExpression>;
template class Evmdd<TupleExpression>;

template class EvmddFactory<NumericExpression>;
template class EvmddFactory<VariableAssignementExpression>;
template class EvmddFactory<TupleExpression>;
