#ifndef NUMERIC_CATAMORPH_EVMDD_H
#define NUMERIC_CATAMORPH_EVMDD_H

#include <map>
#include <memory>
#include <vector>

enum EVMDD_TYPE { SDAC, ENF, COMBINED };

template <typename T>
class Label {
public:
    std::unique_ptr<T> expression;
    Label(const Label &that) = delete;
    Label(){};
};

template <typename T>
class Edge;

template <typename T>
class Node {
public:
    int id;
    std::string variable;
    std::vector<std::shared_ptr<Edge<T>>> outgoing; // outgoing edge
    std::vector<std::shared_ptr<Edge<T>>> incoming; // incoming edge
    std::vector<std::shared_ptr<T>>
        partial_evaluations; // current value of evaluation
    template <typename EvaluationFunction>
    void add_partial_evaluation(
        Edge<T> const &incoming_edge,
        EvaluationFunction evaluationFunction = EvaluationFunction());

    Node(const Node<T> &that) = delete;

    Node(){};
};

template <typename T>
class Edge {
public:
    unsigned int value;
    std::shared_ptr<Node<T>> predecessor;
    std::shared_ptr<Node<T>> successor;
    std::unique_ptr<Label<T>> label;
    Edge(const Edge<T> &that) = delete;
    Edge() : predecessor(nullptr){};
};

template <typename T>
class Evmdd {
public:
    // Evmdd(const Evmdd &that) = delete;
    Evmdd(){};
    std::shared_ptr<Node<T>> terminal;
    std::shared_ptr<Edge<T>> evmdd;
    std::map<int, std::shared_ptr<Node<T>>> nodes;
    std::vector<std::shared_ptr<T>> evaluate_partial(
        std::map<std::string, std::vector<int>> const &state) const;
    template <typename EvaluationFunction>
    std::vector<std::shared_ptr<T>> evaluate(
        std::map<std::string, std::vector<int>> const &state,
        EvaluationFunction evaluationFunction = EvaluationFunction()) const;
    std::vector<std::shared_ptr<T>> get_min() const;
    std::vector<std::shared_ptr<T>> get_max() const;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
