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
private:
    static std::map<size_t, Node<T>> memorized;
    // static Node<T> terminal;
    // static Node<T> dummy;
    Node();

public:
    static int id_counter;
    int id;
    int level;
    std::string variable;
    std::vector<Edge<T>> outgoing; // outgoing edge
    std::vector<Edge<T>> incoming; // incoming edge
    // Node(const Node<T> &that) = delete;
    // Node();
    static Node<T> makeNode(std::vector<Edge<T>> outgoing, std::string variable,
                            int level);
    static Node<T> makeTerminalNode();

    static Node<T> makeDummyNode();
};

template <typename T>
std::map<size_t, Node<T>> Node<T>::memorized;
template <typename T>
int Node<T>::id_counter = 1;

template <typename T>
class Edge {
public:
    unsigned int value;
    Node<T> successor;
    Node<T> predecessor;
    Label<T> label;
    // Edge(const Edge<T> &that) = idelete;
    Edge(Node<T> suc = Node<T>::makeTerminalNode(),
         Node<T> pre = Node<T>::makeDummyNode());
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
    static Edge<T> makeConstEvmdd(T weight, unsigned int edge_value);
    static Edge<T> makeVarEvmdd(const std::string var, unsigned int domain,
                                int level);

    // Evmdd(const Evmdd &that) = delete;
    // Evmdd(){};
    // Node<T> terminal;
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
    void print();
    void print_rec(Edge<T> edge, std::string depth);
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
