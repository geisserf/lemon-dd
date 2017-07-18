#include "Evmdd.h"
#include "EvmddExpression.h"
#include "Expression.h"
#include "Factories.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
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
    // std::cout << std::endl << "Begin Eval:: " << std::endl;
    std::map<int, std::vector<T>> partialEvaluations;

    std::queue<Edge<T>> openList;
    openList.push(evmdd);
    Edge<T> currentEdge;
    while (openList.size() > 0) {
        currentEdge = openList.front();
        // std::cout << "currentEdge " <<
        // currentEdge.label.expression.toString()
        //           << " from " << currentEdge.predecessor.variable << " id "
        //           << currentEdge.predecessor.id << " to "
        //           << currentEdge.successor.variable
        //           << " id: " << currentEdge.successor.id << std::endl;
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

        //      std::cout<<"added res: "<<res[0].toString()<<std::endl;

        if (!currentEdge.successor.outgoing.empty()) {
            if (state.find(currentEdge.successor.variable) != state.end()) {
                // variable is in state,  add successor outgoing edges to open
                // list
                //              std::cout << "get State values from "
                //                        << currentEdge.successor.variable <<
                //                        std::endl;
                auto const &state_values =
                    state.at(currentEdge.successor.variable);
                for (const auto &state_value : state_values) {
                    //                std::cout << " value" << std::endl;
                    if (currentEdge.successor.outgoing.size() >
                            (unsigned)state_value &&
                        state_value >= 0) {
                        //                  std::cout << "find_if" << std::endl;
                        for (auto e : currentEdge.successor.outgoing) {
                            //                    std::cout << "checking Value "
                            //                    << e.value
                            //                              << " against " <<
                            //                              state_value
                            //                              << std::endl;
                            if (e.value == (unsigned)state_value) {
                                openList.push(e);

                                //                      std::cout << "pushed" <<
                                //                      std::endl;
                            }
                        }

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
                    openList.push(succ_edge);
                }
            }
        }
    }
    //    std::cout << "finished" << std::endl;
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
        //      std::cout<<"partial evaluate no predecessor"<<std::endl;
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

        std::vector<T> predecessor_partial_evaluations;
        if (pe != partialEvaluations.end()) {
            predecessor_partial_evaluations = pe->second;
        } // else{
        // std::cout<<"partial_evals empty, predecessor not done"<<std::endl;
        //}

        // std::vector<T> partial_evaluations =
        //     partialEvaluations.find(incoming_edge.predecessor.id)->second;
        // execute evaluation function and add to partial_evaluation

        std::vector<T> result;
        auto current_pe = partialEvaluations.find(incoming_edge.successor.id);
        if (current_pe != partialEvaluations.end()) {
            result = current_pe->second;
        }

        for (auto &it : predecessor_partial_evaluations) {
            T current = it + incoming_edge.label.expression;
            // auto x = current.evaluation_function(partial_evaluations,
            // compare);
            //             std::cout<<"execute eval funciton on
            //             "<<current.toString()<<std::endl;
            auto x = evaluationFunction(current, result);
            //    for(auto a:x){
            //           std::cout<<"partial eval: "<<a.toString()<<std::endl;
            //    }
            result = x;
        }
        return result;
    }
}

template <typename T>
Node<T>::Node() {
    id = id_counter++;
    variable = "";
}

template <typename T>
Evmdd<T> Evmdd<T>::apply(Evmdd<T> const &other,
                         expression_r<Evmdd<T>> const &oper) {
    // std::cout<<"APPLY"<<std::endl;
    this->evmdd = _apply(this->evmdd, other.evmdd, oper);

    return *this;
}

template <typename T>
Edge<T> Evmdd<T>::_apply(Edge<T> const &first, Edge<T> const &other,
                         expression_r<Evmdd<T>> const &oper) {
    //   std::cout << "_apply" << std::endl;

    //      std::cout << "self var: " <<
    //      first.successor.variable<<first.successor.id
    //                << " other var: " <<
    //                other.successor.variable<<other.successor.id << std::endl;
    // terminal case
    if (first.successor.id == 0 && other.successor.id == 0) {
        //        std::cout<<"Terminal"<<std::endl;
        return _terminal_value(first, other, oper);
    }
    // get the higher level
    int level = std::max(first.successor.level, other.successor.level);
    // align edges
    // std::cout<<"align"<<std::endl;
    auto self_children = _align_levels(first, other);
    auto other_children = _align_levels(other, first);
    // std::cout<<"end align"<<std::endl;
    std::vector<Edge<T>> children;
    // new children
    for (size_t c = 0; c < self_children.size(); c++) {
        // apply operator on edges of both children
        children.push_back(
            apply_operator(self_children[c], other_children[c], oper));
    }

    // get minimum of child edge labels
    auto rw = std::min_element(
        children.begin(), children.end(), [](Edge<T> e1, Edge<T> e2) {
            return e1.label.expression < e2.label.expression;
        });

    // std::cout<<"go mit"<<std::endl;

    std::string var = "";
    if (first.successor.level == level) {
        var = first.successor.variable;
    } else {
        var = other.successor.variable;
    }

    Node<T> result_succ = Node<T>::makeNode(children, var, level);

    //    std::cout<<"succ build"<<std::endl;
    T result_weight = rw->label.expression;
    // subtract min from children (moved back up)

    for (auto it = result_succ.outgoing.begin();
         it != result_succ.outgoing.end(); it++) {
        //      std::cout<<"subtracting"<<it->label.expression.toString()<<"-"<<result_weight.toString()<<std::endl;
        it->label.expression = it->label.expression - result_weight;
        // it->predecessor = result_succ;
    }

    // std::cout<<"subtracted min"<<std::endl;
    // Shannon reduction
    // TODO

    Edge<T> res(result_succ);

    //  res.successor = result_succ;
    res.label.expression = result_weight;
    res.value = first.value;

    // std::cout<<"done . return"<<std::endl;
    // std::cout<<"Result: "<<res.predecessor.variable<<" to
    // "<<res.successor.variable<< " with label
    // "<<res.label.expression.toString()<<std::endl;
    return res;
}

template <typename T>
Edge<T> Evmdd<T>::apply_operator(Edge<T> edge1, Edge<T> edge2,
                                 expression_r<Evmdd<T>> const &oper) {
    return _apply(edge1, edge2, oper);
}

template <typename T>
std::vector<Edge<T>> Evmdd<T>::_align_levels(Edge<T> edge1, Edge<T> edge2) {
    //  std::cout << "aligning: edge " << edge1.predecessor.variable << ","
    //            << edge1.successor.variable << ":"
    //            << edge1.label.expression.toString() << " and "
    //            << edge2.predecessor.variable << "," <<
    //            edge2.successor.variable
    //            << ":" << edge2.label.expression.toString() << std::endl;

    std::vector<Edge<T>> res;
    // std::cout << " edge1 " << edge1.successor.level << " edge2 "
    //           << edge2.successor.level << std::endl;
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

    return res;
}

template <typename T>
// template <typename Operator>
Edge<T> Evmdd<T>::_terminal_value(Edge<T> first, Edge<T> other,
                                  expression_r<Evmdd<T>> const &oper) {
    if (Factories::get_as_add(oper)) {
        return Evmdd<T>::makeConstEvmdd(
            first.label.expression + other.label.expression, first.value);
    } else if (Factories::get_as_sub(oper)) {
        return Evmdd<T>::makeConstEvmdd(
            first.label.expression - other.label.expression, first.value);
    } else if (Factories::get_as_mul(oper)) {
        return Evmdd<T>::makeConstEvmdd(
            first.label.expression * other.label.expression, first.value);
    } else if (Factories::get_as_div(oper)) {
        return Evmdd<T>::makeConstEvmdd(
            first.label.expression / other.label.expression, first.value);
    }
    throw std::logic_error("Unknown Operator in Apply");
}

template <>
Edge<NumericExpression> Evmdd<NumericExpression>::makeVarEvmdd(
    const std::string var, unsigned int domain, int level) {
    // terminal node
    Node<NumericExpression> terminal =
        Node<NumericExpression>::makeTerminalNode();

    // Incoming
    Label<NumericExpression> incoming_label;
    NumericExpression expression;
    expression.value = 0;
    incoming_label.expression = expression;

    // Variable Node

    // for each domain value add edge from incoming_edge.get()->successor to
    // terminal with weight = domain value
    std::vector<Edge<NumericExpression>> outgoing;
    for (unsigned int d = 0; d < domain; d++) {
        Label<NumericExpression> edge_label;
        NumericExpression edge_expression;
        edge_expression.value = d;
        edge_label.expression = edge_expression;
        Edge<NumericExpression> edge;
        edge.label = edge_label;
        edge.value = d;
        outgoing.push_back(edge);
    }

    Node<NumericExpression> variable_node =
        Node<NumericExpression>::makeNode(outgoing, var, level);

    Edge<NumericExpression> incoming_edge(variable_node);
    incoming_edge.label = incoming_label;
    incoming_edge.value = 0;
    variable_node.incoming.push_back(incoming_edge);
    //    std::cout<<"Edge from
    //    "<<incoming_edge.predecessor.variable<<":"<<incoming_edge.predecessor.id<<
    //    " to
    //    "<<incoming_edge.successor.variable<<":"<<incoming_edge.successor.id<<std::endl;
    return incoming_edge;
}

template <typename T>
Edge<T> Evmdd<T>::makeConstEvmdd(T weight) {
    return makeConstEvmdd(weight, 0);
}

template <typename T>
Edge<T> Evmdd<T>::makeConstEvmdd(T weight, unsigned int edge_value) {
    // Node<T> terminal = Node<T>::make
    //    std::cout<<"create const"<<std::endl;
    Edge<T> edge;
    Label<T> l;
    l.expression = weight;
    edge.label = l;
    edge.value = edge_value;
    // std::cout<<"Const-Edge from
    // "<<edge.predecessor.variable<<":"<<edge.predecessor.id<< " to
    // "<<edge.successor.variable<<":"<<edge.successor.id<<std::endl;i
    //  std::cout<<"   done"<<std::endl;
    return edge;
}

template <typename T>
Node<T> Node<T>::makeTerminalNode() {
    std::vector<Edge<T>> outgoing;
    Node<T> t = Node<T>::makeNode(outgoing, "", 0);
    t.id = 0;
    return t;
}

template <typename T>
Node<T> Node<T>::makeDummyNode() {
    std::vector<Edge<T>> outgoing;
    Node<T> dummy = Node<T>::makeNode(outgoing, "", 0);
    return dummy;
}

template <typename T>
Node<T> Node<T>::makeNode(std::vector<Edge<T>> outgoing, std::string variable,
                          int level) {
    size_t hash_ = (size_t)level;
    size_t seed = 0;
    size_t size = outgoing.size();

    for (size_t i = 0; i < size; i++) {
        hash_ ^=
            std::hash<std::string>{}(outgoing[i].label.expression.toString()) +
            0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    auto element = Node<T>::memorized.find(hash_);
    if (element != Node<T>::memorized.end()) {
        //   std::cout<<"Return existing Node: "<<variable<<std::endl;
        //    return element->second;
    }

    Node<T> node;
    node.level = level;
    node.variable = variable;
    node.outgoing = outgoing;
    for (auto it = node.outgoing.begin(); it != node.outgoing.end(); it++) {
        it->predecessor = node;
    }

    Node<T>::memorized.insert(std::pair<size_t, Node<T>>(hash_, node));
    // std::cout<<"Return new node: "<<variable<<std::endl;
    return node;
}

template <typename T>
void Evmdd<T>::print() {
    print_rec(this->evmdd, "");
}

template <typename T>
void Evmdd<T>::print_rec(Edge<T> edge, std::string depth) {
    std::cout << depth << "from: " << edge.predecessor.variable
              << " with label " << edge.label.expression.toString()
              << " to: " << edge.successor.variable << std::endl;
    depth = depth + "  ";
    for (auto child : edge.successor.outgoing) {
        print_rec(child, depth);
    }
}

template <typename T>
Edge<T>::Edge(Node<T> suc, Node<T> pre) : successor(suc), predecessor(pre) {
    suc.incoming.push_back(*this);
    pre.outgoing.push_back(*this);
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
