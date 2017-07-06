#ifndef NUMERIC_CATAMORPH_EVMDD_H
#define NUMERIC_CATAMORPH_EVMDD_H

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
public:
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

    std::vector<T> get_min() const;
    std::vector<T> get_max() const;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
