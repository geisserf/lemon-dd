#include "evmdd.h"
#include "evmdd_expression.h"
#include "node.h"
#include <iostream>
#include <queue>

using std::vector;
using std::map;
using std::string;
using std::shared_ptr;

template <typename T>
vector<T> Evmdd<T>::get_min() {
    return evaluate(map<string, vector<int>>(), greates_lower_bound<T>());
}

template <typename T>
vector<T> Evmdd<T>::get_max() {
    return evaluate(map<string, vector<int>>(), least_upper_bound<T>());
}

template <typename T>
vector<T> Evmdd<T>::evaluate_partial(map<string, vector<int>> const &state) {
    return evaluate(state, greates_lower_bound<T>());
}

template <typename T>
template <typename EvaluationFunction>
vector<T> Evmdd<T>::evaluate(State const &state,
                             EvaluationFunction eval_function) {
    vector<T> per_state_result = entry_node->evaluate(state, eval_function);
    vector<T> result;
    for (T res : per_state_result) {
        T current = res + input_value;
        result = eval_function(current, result);
    }
    return result;
}

template <typename T>
Evmdd<T>::Evmdd(T input, shared_ptr<Node<T> const> entry_node)
    : input_value(input), entry_node(entry_node) {}

template <typename T>
Evmdd<T> EvmddFactory<T>::make_const_evmdd(T weight) {
    return Evmdd<T>(weight, node_factory.get_terminal_node());
}

template <typename T>
Evmdd<T> EvmddFactory<T>::make_var_evmdd(string const &var,
                                         vector<T> const &domain) {
    vector<Edge<T>> children;
    for (size_t i = 0; i < domain.size(); ++i) {
        children.emplace_back(T(domain[i]), node_factory.get_terminal_node());
    }
    shared_ptr<Node<T> const> node =
        node_factory.make_node(ordering[var], var, children);
    return Evmdd<T>(T::identity(), node);
}

template class Evmdd<NumericExpression>;
template class Evmdd<VariableAssignmentExpression>;
template class Evmdd<TupleExpression>;

template class EvmddFactory<NumericExpression>;
template class EvmddFactory<VariableAssignmentExpression>;
template class EvmddFactory<TupleExpression>;
