#include "evmdd.h"
#include "evmdd_expression.h"
#include "node.h"
#include <iostream>
#include <queue>

using std::vector;
using std::string;

template <typename T>
std::vector<T> Evmdd<T>::get_min() {
    return evaluate(std::map<std::string, std::vector<int>>(),
                    greates_lower_bound<T>());
}

template <typename T>
std::vector<T> Evmdd<T>::get_max() {
    return evaluate(std::map<std::string, std::vector<int>>(),
                    least_upper_bound<T>());
}

template <typename T>
std::vector<T> Evmdd<T>::evaluate_partial(
    std::map<std::string, std::vector<int>> const &state) {
    return evaluate(state, greates_lower_bound<T>());
}

template <typename T>
template <typename EvaluationFunction>
std::vector<T> Evmdd<T>::evaluate(State const &state,
                                  EvaluationFunction evaluationFunction) {
    if (entry_node.get_id() == 0) {
        std::vector<T> result = {{input_value}};
        return result;
    }

    auto res = entry_node.evaluate(state, evaluationFunction);
    std::vector<T> result; // = {{input_value}};
    for (auto e : res) {
        T current = e + input_value;
        result = evaluationFunction(current, result);
    }
    return result;
}

template <typename T>
Evmdd<T>::Evmdd(T input, Node<T> const &entry_node)
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
        children.emplace_back(Label<T>(domain[i]), 0);
    }
    Node<T> node = node_factory.make_node(ordering[var], var, children);
    return Evmdd<T>(T::identity(), node);
}

template class Evmdd<NumericExpression>;
template class Evmdd<VariableAssignmentExpression>;
template class Evmdd<TupleExpression>;

template class EvmddFactory<NumericExpression>;
template class EvmddFactory<VariableAssignmentExpression>;
template class EvmddFactory<TupleExpression>;
