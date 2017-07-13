#ifndef NUMERIC_CATAMORPH_EVMDD_H
#define NUMERIC_CATAMORPH_EVMDD_H

#include "Expression.h"
#include <map>
#include <memory>
#include <vector>

enum EVMDD_TYPE { SDAC, ENF, COMBINED };

template <typename T>
class Label {
public:
    T expression;
    // Label(const Label &that) = delete;
    // Label(){};
};

template <typename T>
class Edge;

template <typename T>
class Node {
public:
    static int id_counter;
    int id;
    int level;
    std::string variable;
    std::vector<Edge<T>> outgoing; // outgoing edge
    std::vector<Edge<T>> incoming; // incoming edge
    // Node(const Node<T> &that) = delete;
    Node();
};

template <typename T>
class Edge {
public:
    unsigned int value;
    Node<T> predecessor;
    Node<T> successor;
    Label<T> label;
    // Edge(const Edge<T> &that) = delete;
};

template <typename T>
class Evmdd {
private:
    // template <typename Operator>
    Edge<T> _terminal_value(Edge<T> first, Edge<T> other,
                            expression_r<Evmdd<T>> const &oper);
    std::vector<Edge<T>> _align_levels(Edge<T> edge1, Edge<T> edge2);
    Edge<T> apply_operator(Edge<T> edge1, Edge<T> edge2,
                           expression_r<Evmdd<T>> const &oper);
    Edge<T> _apply(Edge<T> const &first, Edge<T> const &other,
                   expression_r<Evmdd<T>> const &oper);

public:
    static Edge<T> makeConstEvmdd(T weight);
    static Edge<T> makeVarEvmdd(const std::string var, unsigned int domain,
                                int level);

    // Evmdd(const Evmdd &that) = delete;
    // Evmdd(){};
    Node<T> terminal;
    Edge<T> evmdd;
    std::vector<Node<T>> nodes;
    std::vector<T> evaluate_partial(
        std::map<std::string, std::vector<int>> const &state) const;

    template <typename EvaluationFunction>
    std::vector<T> evaluate(
        std::map<std::string, std::vector<int>> const &state,
        EvaluationFunction evaluationFunction = EvaluationFunction()) const;

    template <typename EvaluationFunction>
    std::vector<T> calculate_partial_evaluation(
        Edge<T> const &incoming_edge,
        std::map<int, std::vector<T>> const &partialEvaluations,
        EvaluationFunction evaluationFunction = EvaluationFunction()) const;

    // template <typename Operator>
    Evmdd<T> apply(Evmdd<T> const &other, expression_r<Evmdd<T>> const &oper);

    std::vector<T> get_min() const;
    std::vector<T> get_max() const;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
