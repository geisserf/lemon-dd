#ifndef NUMERIC_CATAMORPH_EVMDD_H
#define NUMERIC_CATAMORPH_EVMDD_H

#include "node.h"

#include <functional>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

enum EVMDD_TYPE { SDAC, ENF, COMBINED };

using Ordering = std::map<std::string, int>;

template <typename T>
class Node;

template <typename T>
class EvmddFactory;

template <typename T>
class Evmdd {
private:
    T input_value;
    Node<T> entry_node;

    Evmdd(T input, Node<T> const &entry_node);

    friend EvmddFactory<T>;

public:
    void print(std::ostream &out) {
        out << "IMPLEMENT ME";
    }
    std::vector<T> evaluate_partial(
        std::map<std::string, std::vector<int>> const &state) const;

    template <typename EvaluationFunction>
    std::vector<T> evaluate(
        std::map<std::string, std::vector<int>> const &state,
        EvaluationFunction evaluationFunction = EvaluationFunction()) const;

    template <typename EvaluationFunction>
    std::vector<T> calculate_partial_evaluation(
        Node<T> const &current_node, T const &incoming_weight,
        std::vector<T> const &previous_evaluations,
        EvaluationFunction evaluation_function = EvaluationFunction()) const;

    std::vector<T> get_min() const;
    std::vector<T> get_max() const;
};

template <typename T>
class EvmddFactory {
public:
    Evmdd<T> make_const_evmdd(T weight);

    Evmdd<T> make_var_evmdd(int level, std::string const &var, int domain);

    template <typename F>
    Evmdd<T> apply(Evmdd<T> const &first, Evmdd<T> const &second, F oper) {
        (void)(first);
        (void)(second);
        (void)(oper);
        return make_const_evmdd(T{});
    }

private:
    // variable ordering
    Ordering ordering;
    NodeFactory<T> node_factory;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
