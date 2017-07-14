#include "Evmdd.h"
#include "EvmddExpression.h"
#include "Expression.h"
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
    std::cout << std::endl << "Begin Eval:: " << std::endl;
    std::map<int, std::vector<T>> partialEvaluations;

    std::queue<Edge<T>> openList;
    openList.push(evmdd);
    Edge<T> currentEdge;
    while (openList.size() > 0) {
        currentEdge = openList.front();
        std::cout << "currentEdge " << currentEdge.label.expression.toString()
                  << " from " << currentEdge.predecessor.variable << " id "
                  << currentEdge.predecessor.id << " to "
                  << currentEdge.successor.variable
                  << " id: " << currentEdge.successor.id << std::endl;
        openList.pop();
        auto p = partialEvaluations.find(currentEdge.successor.id);
        auto res = calculate_partial_evaluation(currentEdge, partialEvaluations,
                                                evaluationFunction);
        if (p == partialEvaluations.end()) {
            //  std::cout<<"adding pe id: "<<currentEdge.successor.id<<"
            //  entries: "<<res.size()<<std::endl;
            partialEvaluations.insert(
                std::pair<int, std::vector<T>>(currentEdge.successor.id, res));
        } else {
            p->second = res;
        }
        if (!currentEdge.successor.outgoing.empty()) {
            if (state.find(currentEdge.successor.variable) != state.end()) {
                // variable is in state,  add successor outgoing edges to open
                // list
                std::cout << "get State values" << std::endl;
                auto const &state_values =
                    state.at(currentEdge.successor.variable);
                for (const auto &state_value : state_values) {
                    std::cout << " value" << std::endl;
                    if (currentEdge.successor.outgoing.size() >
                            (unsigned)state_value &&
                        state_value >= 0) {
                        std::cout << "find_if" << std::endl;
                        for (auto e : currentEdge.successor.outgoing) {
                            std::cout << "checking Value " << e.value
                                      << " against " << state_value
                                      << std::endl;
                            if (e.value == (unsigned)state_value) {
                                openList.push(e);

                                std::cout << "pushed" << std::endl;
                            }
                        }

                        // auto it = std::find_if(
                        //     currentEdge.successor.outgoing.begin(),
                        //     currentEdge.successor.outgoing.end(),
                        //     [state_value](const Edge<T> &obj) {
                        //         return obj.value == (unsigned)state_value;
                        //     });
                        // std::cout<<"push"<<std::endl;
                        // if(it != currentEdge.successor.outgoing.end()){
                        //   openList.push(*it);
                        // }else{
                        // std::cout<<"not found
                        // "<<currentEdge.successor.variable<<" outgoing value
                        // "<<state_value<<std::endl;
                        // }
                        //                        openList.push(*it);
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
                for (Edge<T> &succ_edge : currentEdge.successor.outgoing) {
                    //                std::cout << "    adding edge: "
                    //                      <<
                    //                      succ_edge.label.expression.toString()
                    //                      << " to "
                    //                        << succ_edge.successor.variable <<
                    //                        std::endl;
                    openList.push(succ_edge);
                }
            }
        }
    }
    std::cout << "finished" << std::endl;
    // if (partialEvaluations.find(terminal.id) != partialEvaluations.end()) {
    // std::cout<<"found"<<std::endl;
    //}
    return partialEvaluations.find(0)->second;
}

template <typename T>
template <typename EvaluationFunction>
std::vector<T> Evmdd<T>::calculate_partial_evaluation(
    Edge<T> const &incoming_edge,
    std::map<int, std::vector<T>> const &partialEvaluations,
    EvaluationFunction evaluationFunction) const {
    //(void)evaluationFunction;
    if (incoming_edge.predecessor.variable == "") {
        //        std::cout<<"partial evaluate no predecessor"<<std::endl;
        T current = incoming_edge.label.expression;
        std::vector<T> result;
        T e;
        e.value = current.value;
        result.push_back(e);
        return result;
    } else {
        //    std::cout<<"partial evaluatei predecessor:
        //    "<<incoming_edge.predecessor.id<<std::endl;
        auto pe = partialEvaluations.find(incoming_edge.predecessor.id);

        std::vector<T> partial_evaluations;
        if (pe != partialEvaluations.end()) {
            partial_evaluations = pe->second;
        } // else{
        // std::cout<<"partial_evals empty, predecessor not done"<<std::endl;
        //}

        // std::vector<T> partial_evaluations =
        //     partialEvaluations.find(incoming_edge.predecessor.id)->second;
        // execute evaluation function and add to partial_evaluation
        for (auto &it : partial_evaluations) {
            T current = it + incoming_edge.label.expression;
            // auto x = current.evaluation_function(partial_evaluations,
            // compare);
            //       std::cout<<"execute eval funciton on
            //       "<<current.toString()<<std::endl;
            auto x = evaluationFunction(current, partial_evaluations);
            //            for(auto a:x){
            //       std::cout<<"partial eval: "<<a.toString()<<std::endl;
            //          }
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
Evmdd<T> Evmdd<T>::apply(Evmdd<T> const &other,
                         expression_r<Evmdd<T>> const &oper) {
    this->evmdd = _apply(this->evmdd, other.evmdd, oper);

    return *this;
}

template <typename T>
Edge<T> Evmdd<T>::_apply(Edge<T> const &first, Edge<T> const &other,
                         expression_r<Evmdd<T>> const &oper) {
    std::cout << "Apply" << std::endl;

    std::cout << "self id: " << first.successor.variable
              << " other id: " << other.successor.variable << std::endl;
    // terminal case
    if (first.successor.id == 0 && other.successor.id == 0) {
        return _terminal_value(first, other, oper);
    }
    // get the higher level
    int level = std::max(first.successor.level, other.successor.level);
    // align edges
    auto self_children = _align_levels(first, other);
    auto other_children = _align_levels(other, first);

    std::vector<Edge<T>> children;
    // new children
    for (size_t c = 0; c < self_children.size(); c++) {
        // apply operator on edges of both children
        children.push_back(
            apply_operator(self_children[c], other_children[c], oper));

        std::cout << "Child after op apply " << children[c].predecessor.variable
                  << "," << children[c].successor.variable << ":"
                  << children[c].label.expression.toString() << std::endl;
    }

    // get minimum of child edge labels
    auto rw = std::min_element(
        children.begin(), children.end(), [](Edge<T> e1, Edge<T> e2) {
            return e1.label.expression < e2.label.expression;
        });

    Node<T> result_succ;
    result_succ.level = level;
    result_succ.outgoing = children;

    if (first.successor.level == level) {
        result_succ.variable = first.successor.variable;
    } else {
        result_succ.variable = other.successor.variable;
    }
    T result_weight = rw->label.expression;
    // subtract min from children (moved back up)

    for (auto it = result_succ.outgoing.begin();
         it != result_succ.outgoing.end(); it++) {
        it->label.expression = it->label.expression - result_weight;
        it->predecessor = result_succ;
    }

    // Shannon reduction
    // TODO

    Edge<T> res;

    res.successor = result_succ;
    res.label.expression = result_weight;
    res.value = first.value;

    // TODO missing rest of evmdd details
    // this->evmdd = result_edge;
    return res;
}

template <typename T>
Edge<T> Evmdd<T>::apply_operator(Edge<T> edge1, Edge<T> edge2,
                                 expression_r<Evmdd<T>> const &oper) {
    // I think :-(?
    // if(!edge2.successor.id==0){
    //   return _apply(edge2,oper);
    // }
    //
    //
    //    T e;
    //    if (Factories::get_as_add(oper)) {
    //        e = edge1.label.expression + edge2.label.expression;
    //    } else if (Factories::get_as_sub(oper)) {
    //        e = edge1.label.expression - edge2.label.expression;
    //    } else if (Factories::get_as_mul(oper)) {
    //        e = edge1.label.expression * edge2.label.expression;
    //    } else if (Factories::get_as_div(oper)) {
    //        e = edge1.label.expression / edge2.label.expression;
    //    } else {
    //        throw std::logic_error("Unknown Operator in Apply");
    //    }
    //
    //    Edge<T> edge = Edge<T>(edge1);
    //   edge.label.expression = e;
    //    return edge;
    return _apply(edge1, edge2, oper);
}

template <typename T>
std::vector<Edge<T>> Evmdd<T>::_align_levels(Edge<T> edge1, Edge<T> edge2) {
    std::cout << "aligning: edge " << edge1.predecessor.variable << ","
              << edge1.successor.variable << ":"
              << edge1.label.expression.toString() << " and "
              << edge2.predecessor.variable << "," << edge2.successor.variable
              << ":" << edge2.label.expression.toString() << std::endl;

    std::vector<Edge<T>> res;
    std::cout << " edge1 " << edge1.successor.level << " edge2 "
              << edge2.successor.level << std::endl;
    if (edge1.successor.level >= edge2.successor.level) {
        for (Edge<T> child : edge1.successor.outgoing) {
            T newexp = child.label.expression + edge1.label.expression;
            Edge<T> ne = Edge<T>(child);
            ne.label.expression = newexp;
            res.push_back(ne);
        }
    } else {
        int value = 0;
        for (Edge<T> child : edge2.successor.outgoing) {
            Edge<T> ne = Edge<T>(edge1);
            ne.value = value;
            value++;
            res.push_back(ne);
        }
    }
    for (auto e : res) {
        std::cout << " New Edge " << e.predecessor.variable << ","
                  << e.successor.variable << ":"
                  << e.label.expression.toString() << " level"
                  << e.successor.level << "Value" << e.value << std::endl;
    }

    return res;
}

template <typename T>
// template <typename Operator>
Edge<T> Evmdd<T>::_terminal_value(Edge<T> first, Edge<T> other,
                                  expression_r<Evmdd<T>> const &oper) {
    if (Factories::get_as_add(oper)) {
        return Evmdd<T>::makeConstEvmdd(first.label.expression +
                                        other.label.expression);
    } else if (Factories::get_as_sub(oper)) {
        return Evmdd<T>::makeConstEvmdd(first.label.expression -
                                        other.label.expression);
    } else if (Factories::get_as_mul(oper)) {
        return Evmdd<T>::makeConstEvmdd(first.label.expression *
                                        other.label.expression);
    } else if (Factories::get_as_div(oper)) {
        return Evmdd<T>::makeConstEvmdd(first.label.expression /
                                        other.label.expression);
    }
    throw std::logic_error("Unknown Operator in Apply");
}

template <>
Edge<NumericExpression> Evmdd<NumericExpression>::makeVarEvmdd(
    const std::string var, unsigned int domain, int level) {
    // terminal node
    Node<NumericExpression> terminal;
    terminal.id = 0;
    terminal.level = 0;
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
    // Evmdd<NumericExpression> result;
    // result.evmdd = incoming_edge;
    // result.terminal = terminal;

    return incoming_edge;
}

template <typename T>
Edge<T> Evmdd<T>::makeConstEvmdd(T weight) {
    Edge<T> edge;
    Label<T> l;
    l.expression = std::move(weight);
    edge.label = std::move(l);
    edge.value = 0;
    Node<T> successor;
    successor.level = 0;
    successor.id = 0;
    edge.successor = successor;
    // Evmdd<T> result;
    // result.terminal = successor;
    // result.evmdd = edge;
    return edge;
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
