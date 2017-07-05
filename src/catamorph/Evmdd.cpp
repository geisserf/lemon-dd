#include "Evmdd.h"
#include "EvmddExpression.h"
#include <algorithm>
#include <iostream>
#include <queue>

template <typename T>
std::vector<std::shared_ptr<T>> Evmdd<T>::get_min() const {
    return evaluate(std::map<std::string, std::vector<int>>(),
                    greates_lower_bound<T>());
}

template <typename T>
std::vector<std::shared_ptr<T>> Evmdd<T>::get_max() const {
    return evaluate(std::map<std::string, std::vector<int>>(),
                    least_upper_bound<T>());
    // return std::vector<std::shared_ptr<T>>();
}

template <typename T>
std::vector<std::shared_ptr<T>> Evmdd<T>::evaluate_partial(
    std::map<std::string, std::vector<int>> const &state) const {
    return evaluate(state, greates_lower_bound<T>());
}

template <typename T>
template <typename EvaluationFunction>
std::vector<std::shared_ptr<T>> Evmdd<T>::evaluate(
    std::map<std::string, std::vector<int>> const &state,
    EvaluationFunction evaluationFunction) const {
    // clear partial evaluations
    for (std::pair<int, std::shared_ptr<Node<T>>> const &node : nodes) {
        node.second->partial_evaluations
            .clear(); //.second->partial_evaluations.clear();
    }

    std::queue<std::shared_ptr<Edge<T>>> openList;
    openList.push(evmdd);
    std::shared_ptr<Edge<T>> currentEdge;
    while (openList.size() > 0) {
        currentEdge = openList.front();
        openList.pop();

        // add partial evaluation to successor
        currentEdge->successor->add_partial_evaluation(*currentEdge,
                                                       evaluationFunction);

        if (!currentEdge->successor->outgoing.empty()) {
            if (state.find(currentEdge->successor->variable) != state.end()) {
                // variable is in state,  add successor outgoing edges to open
                // list
                auto const &state_values =
                    state.at(currentEdge->successor->variable);

                for (const auto &state_value : state_values) {
                    if (currentEdge->successor->outgoing.size() >
                            (unsigned)state_value &&
                        state_value >= 0) {
                        auto it = std::find_if(
                            currentEdge->successor->outgoing.begin(),
                            currentEdge->successor->outgoing.end(),
                            [state_value](const std::shared_ptr<Edge<T>> &obj) {
                                return obj->value == (unsigned)state_value;
                            });
                        openList.push((*it));
                    } else {
                        std::cerr << "Error Evaluating EVMDD: "
                                  << currentEdge->successor->variable
                                  << " with value: " << state_value
                                  << std::endl;
                    }
                }

            } else {
                // variable not in state
                // so we assume that variable can take on all values
                for (const std::shared_ptr<Edge<T>> &succ_edge :
                     currentEdge->successor->outgoing) {
                    openList.push(succ_edge);
                }
            }
        }
    }

    return this->terminal->partial_evaluations;
}

template <typename T>
template <typename EvaluationFunction>
void Node<T>::add_partial_evaluation(Edge<T> const &incoming_edge,
                                     EvaluationFunction evaluationFunction) {
    (void)evaluationFunction;
    // incoming edge has no predecessor,
    // partial_evaluation = edge value
    if (!incoming_edge.predecessor) {
        T current = *(incoming_edge.label.get()->expression);
        std::vector<std::shared_ptr<T>> result;
        auto e = std::shared_ptr<T>(new T());
        e->value = current.value;
        result.push_back(e);
        partial_evaluations.clear();
        partial_evaluations = result;
    } else {
        // execute evaluation function and add to partial_evaluation
        for (const auto &it : incoming_edge.predecessor->partial_evaluations) {
            T current = (*it) + *(incoming_edge.label->expression);
            // auto x = current.evaluation_function(partial_evaluations,
            // compare);
            auto x = evaluationFunction(current, partial_evaluations);
            partial_evaluations = x;
        }
    }
}

template <>
Node<EvmddExpression<int>>::Node() {
    auto a = std::shared_ptr<EvmddExpression<int>>(new EvmddExpression<int>());
    a->value = std::numeric_limits<int>::max();
    partial_evaluations.push_back(a);
}

template class Evmdd<NumericExpression>;

template class Evmdd<VariableAssignementExpression>;

template class Evmdd<TupleExpression>;