#include "Evmdd.h"
#include "EvmddExpression.h"
#include "Factories.h"
#include <algorithm>
#include <iostream>
#include <queue>

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

template <typename T>
template <typename Operator>
Evmdd<T> Evmdd<T>::apply(Evmdd<T> other, Expression oper) {
    // terminal case
    if (this->evmdd.successor.id == this->terminal.id &&
        other.evmdd.successor.id == other.terminal.id) {
        return _termianl_value(other, oper);
    }

    // get the higher level
    int level =
        std::max(this->evmdd.successor.level, other.evmdd.successor.level);
    // align edges
    auto self_children = _align_levels(this->evmdd, other.evmdd);
    auto other_children = _align_levels(other.evmdd, this->evmdd);

    std::vector<Edge<T>> children;
    // new children
    for (int c = 0; c < self_children.size(); c++) {
        // apply operator on edges of both children
        children.push_back(
            apply_operator(self_children[c], other_children[c], oper));
    }

    // get minimum of child edge labels
    T result_weight =
        std::min_element(children.begin(), children.end(), [](Edge<T> e1,
                                                              Edge<T> e2) {
            return e1.label.expression < e2.label.expression;
        }).label.expression;

    // subtract min from children (moved back up)
    for (Edge<T> child : children) {
        child.label.expression = child.label.expression - result_weight;
    }

    // new resulting node
    Node<T> result_succ;
    result_succ.level = level;
    result_succ.outgoing = children;
    // get variable assignet to level (must be from this or other)
    if (this->evmdd.successor.level == level) {
        result_succ.variable = this->evmdd.successor.variable;
    } else {
        result_succ.variable = other.evmdd.successor.variable;
    }

    // Shannon reduction
    // TODO

    // result
    Edge<T> result_edge;
    result_edge.successor = result_succ;
    result_edge.label.expression = result_weight;
    result_edge.value = 0;
    Evmdd<T> result;
    result.evmdd = result_edge;

    // TODO missing rest of evmdd details

    return result;
}

template <typename T>
Edge<T> Evmdd<T>::apply_operator(Edge<T> edge1, Edge<T> edge2,
                                 Expression oper) {
    T e;
    if (Factories::get_as_add(oper.get())) {
        e = edge1.label.expression + edge2.label.expression;
    } else if (Factories::get_as_sub(oper.get())) {
        e = edge1.label.expression - edge2.label.expression;
    } else if (Factories::get_as_mul(oper.get())) {
        e = edge1.label.expression * edge2.label.expression;
    } else if (Factories::get_as_div(oper.get())) {
        e = edge1.label.expression / edge2.label.expression;
    } else {
        throw std::logic_error("Unknown Operator in Apply");
    }

    Edge<T> edge = Edge<T>(edge1);
    edge.label.expression = e;
    return edge;
}

template <typename T>
std::vector<Edge<T>> Evmdd<T>::_align_levels(Edge<T> edge1, Edge<T> edge2) {
    if (edge1.successor.level >= edge2.successor.level) {
        std::vector<Edge<T>> res;
        for (Edge<T> child : edge1.successor.outgoing) {
            T newexp = child.label.expression + edge1.label.expression;
            Edge<T> ne = Edge<T>(child);
            ne.label.expression = newexp;
            res.push_back(ne);
        }
        return res;
    } else {
        std::vector<Edge<T>> res;
        for (Edge<T> child : edge2.successor.outgoing) {
            Edge<T> ne = Edge<T>(edge1);
            res.push_back(ne);
        }
        return res;
    }
}

template <typename T>
template <typename Operator>
Evmdd<T> Evmdd<T>::_termianl_value(Evmdd<T> other, Expression oper) {
    if (Factories::get_as_add(oper.get())) {
        return Evmdd<T>::makeConstEvmdd(this->evmdd.label.expression +
                                        other.evmdd.label.expression);
    } else if (Factories::get_as_sub(oper.get())) {
        return Evmdd<T>::makeConstEvmdd(this->evmdd.label.expression -
                                        other.evmdd.label.expression);
    } else if (Factories::get_as_mul(oper.get())) {
        return Evmdd<T>::makeConstEvmdd(this->evmdd.label.expression *
                                        other.evmdd.label.expression);
    } else if (Factories::get_as_div(oper.get())) {
        return Evmdd<T>::makeConstEvmdd(this->evmdd.label.expression /
                                        other.evmdd.label.expression);
    }
    throw std::logic_error("Unknown Operator in Apply");
}

template <>
Evmdd<NumericExpression> Evmdd<NumericExpression>::makeVarEvmdd(
    const std::string var, unsigned int domain, int level) {
    // terminal node
    Node<NumericExpression> terminal;

    // Incoming Edge
    Label<NumericExpression> incoming_label;
    NumericExpression expression;
    expression.value = 0;
    incoming_label.expression = expression;
    Edge<NumericExpression> incoming_edge;
    incoming_edge.label = incoming_label;
    incoming_edge.value = 0;

    // Variable Node
    Node<NumericExpression> variable_node;
    variable_node.variable = var;
    variable_node.incoming.push_back(incoming_edge);
    variable_node.level = level;

    // for each domain value add edge from incoming_edge.get()->successor to
    // terminal with weight = domain value
    for (unsigned int d = 0; d < domain; d++) {
        Label<NumericExpression> edge_label;
        NumericExpression edge_expression;
        edge_expression.value = d;
        edge_label.expression = edge_expression;
        Edge<NumericExpression> edge;
        edge.label = edge_label;
        edge.value = d;
        edge.successor = terminal;
        edge.predecessor = variable_node;
        variable_node.outgoing.push_back(edge);
    }

    incoming_edge.successor = variable_node;
    Evmdd<NumericExpression> result;
    result.evmdd = incoming_edge;
    result.terminal = terminal;

    return result;
}

template <typename T>
Evmdd<T> Evmdd<T>::makeConstEvmdd(T weight) {
    Edge<T> edge;
    Label<T> l;
    l.expression = std::move(weight);
    edge.label = std::move(l);
    edge.value = 0;
    Node<T> successor;
    successor.level = 0;
    edge.successor = successor;
    Evmdd<T> result;
    result.terminal = successor;
    result.evmdd = edge;
    return result;
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