#include "evmdd.h"
#include "evmdd_expression.h"
#include <iostream>
#include <queue>

using std::vector;
using std::string;

template <typename T>
std::vector<T> Evmdd<T>::get_min() const {
    return evaluate(std::map<std::string, std::vector<int>>(),
                    greates_lower_bound<T>());
}

template <typename T>
std::vector<T> Evmdd<T>::get_max() const {
    return evaluate(std::map<std::string, std::vector<int>>(),
                    least_upper_bound<T>());
}

template <typename T>
std::vector<T> Evmdd<T>::evaluate_partial(
    std::map<std::string, std::vector<int>> const &state) const {
    return evaluate(state, greates_lower_bound<T>());
}

template <typename T>
template <typename EvaluationFunction>
std::vector<T> Evmdd<T>::evaluate(
    std::map<std::string, std::vector<int>> const &state,
    EvaluationFunction evaluationFunction) const {
    std::map<int, std::vector<T>> partialEvaluations;
    partialEvaluations[0] = std::vector<T>{{input_value}};
    NodeStorage<T> node_storage; // = NodeStorage<T>::getInstance();
    std::queue<Node<T>> openList;
    openList.push(entry_node);
    Node<T> currentNode = entry_node;
    while (openList.size() > 0) {
        currentNode = openList.front();
        openList.pop();

        // add weight of edges to child nodes partial evaluations
        std::vector<T> current_eval = partialEvaluations[currentNode.get_id()];
        for (Edge<T> child : currentNode.get_children()) {
            partialEvaluations[child.second] = calculate_partial_evaluation(
                child.first.expression, current_eval, evaluationFunction);
        }

        // add nodes to open list:

        // get child node
        int child_value = 0;
        std::cout << "Current node: " << currentNode.get_variable()
                  << " children " << currentNode.get_children().size()
                  << std::endl;
        for (Edge<T> edge : currentNode.get_children()) {
            Node<T> child = node_storage.get_node(edge.second);
            auto s = state.find(child.get_variable());
            if (s != state.end()) {
                // variable in state:
                for (int state_value : s->second) {
                    std::cout << "Checking state value" << state_value
                              << std::endl;
                    // Node<T> n = node_storage.get_node(
                    //    currentNode.get_children()[state_value].second);
                    // std::cout << "Adding Node: " << n.get_variable()
                    //          << std::endl;
                    if (state_value == child_value) {
                        std::cout << "Adding Node: " << child.get_variable()
                                  << std::endl;
                        openList.push(child);
                    }
                }
            } else {
                // variabe not in state take all possible values
                std::cout << "Adding Node: " << child.get_variable()
                          << std::endl;

                openList.push(child);
            }
            child_value++;
        }
    }
    return partialEvaluations.find(0)->second;
}
//
template <typename T>
template <typename EvaluationFunction>
std::vector<T> Evmdd<T>::calculate_partial_evaluation(
    T const &incoming_weight, std::vector<T> const &previous_evaluations,
    EvaluationFunction evaluation_function) const {
    std::vector<T> result;
    for (auto it : previous_evaluations) {
        T current = it + incoming_weight;
        result = evaluation_function(current, result);
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
