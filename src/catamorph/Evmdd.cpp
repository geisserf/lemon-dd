#include "Evmdd.h"
#include "EvmddExpression.h"
#include <algorithm>
#include <iostream>
#include <queue>

template <typename T>
std::vector<T> Evmdd<T>::get_min() const {
    std::cout << "Get MIN" << std::endl;
    return evaluate(std::map<std::string, std::vector<int>>(),
                    greates_lower_bound<T>());
}

template <typename T>
std::vector<T> Evmdd<T>::get_max() const {
    std::cout << "Get MAX" << std::endl;
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

    std::queue<Edge<T>> openList;
    openList.push(evmdd);
    Edge<T> currentEdge;
    while (openList.size() > 0) {
        currentEdge = openList.front();
        openList.pop();
        auto p = partialEvaluations.find(currentEdge.successor.id);
        auto res = calculate_partial_evaluation(currentEdge, partialEvaluations,
                                                evaluationFunction);
        if (p == partialEvaluations.end()) {
            partialEvaluations.insert(
                std::pair<int, std::vector<T>>(currentEdge.successor.id, res));
        } else {
            p->second = res;
        }

        if (!currentEdge.successor.outgoing.empty()) {
            if (state.find(currentEdge.successor.variable) != state.end()) {
                // variable is in state,  add successor outgoing edges to open
                // list
                auto const &state_values =
                    state.at(currentEdge.successor.variable);

                for (const auto &state_value : state_values) {
                    if (currentEdge.successor.outgoing.size() >
                            (unsigned)state_value &&
                        state_value >= 0) {
                        auto it = std::find_if(
                            currentEdge.successor.outgoing.begin(),
                            currentEdge.successor.outgoing.end(),
                            [state_value](const Edge<T> &obj) {
                                return obj.value == (unsigned)state_value;
                            });
                        openList.push((*it));
                    } else {
                        std::cerr << "Error Evaluating EVMDD: "
                                  << currentEdge.successor.variable
                                  << " with value: " << state_value
                                  << std::endl;
                    }
                }
            } else {
                // variable not in state
                // so we assume that variable can take on all values
                for (const Edge<T> &succ_edge :
                     currentEdge.successor.outgoing) {
                    openList.push(succ_edge);
                }
            }
        }
    }

    return partialEvaluations.find(terminal.id)->second;
}

template <typename T>
template <typename EvaluationFunction>
std::vector<T> Evmdd<T>::calculate_partial_evaluation(
    Edge<T> const &incoming_edge,
    std::map<int, std::vector<T>> const &partialEvaluations,
    EvaluationFunction evaluationFunction) const {
    (void)evaluationFunction;
    if (incoming_edge.predecessor.variable == "") {
        T current = incoming_edge.label.expression;
        std::vector<T> result;
        T e;
        e.value = current.value;
        result.push_back(e);
        return result;
    } else {
        std::vector<T> partial_evaluations =
            partialEvaluations.find(incoming_edge.predecessor.id)->second;
        // execute evaluation function and add to partial_evaluation
        for (auto &it : partial_evaluations) {
            T current = it + incoming_edge.label.expression;
            // auto x = current.evaluation_function(partial_evaluations,
            // compare);
            auto x = evaluationFunction(current, partial_evaluations);
            partial_evaluations = x;
        }
        return partial_evaluations;
    }
}
template <typename T>
int Node<T>::id_counter = 0;

template <typename T>
Node<T>::Node() {
    id = id_counter++;
    variable = "";
}

template class Evmdd<NumericExpression>;
template class Evmdd<VariableAssignementExpression>;
template class Evmdd<TupleExpression>;
template class Node<NumericExpression>;
template class Node<VariableAssignementExpression>;
template class Node<TupleExpression>;
template class Edge<NumericExpression>;
template class Edge<VariableAssignementExpression>;
template class Edge<TupleExpression>;