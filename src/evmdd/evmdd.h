#ifndef NUMERIC_CATAMORPH_EVMDD_H
#define NUMERIC_CATAMORPH_EVMDD_H

#include "node.h"

#include <algorithm>
#include <cassert>
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
        out << "input value: " << input_value.toString();
        out << " nodes: " << std::endl;
        entry_node.print(out);
    }

    std::vector<T> evaluate_partial(
        std::map<std::string, std::vector<int>> const &state);

    template <typename EvaluationFunction>
    std::vector<T> evaluate(
        std::map<std::string, std::vector<int>> const &state,
        EvaluationFunction evaluationFunction = EvaluationFunction());

    std::vector<T> get_min();
    std::vector<T> get_max();
};

template <typename T>
class EvmddFactory {
public:
    void set_ordering(Ordering const &o) {
        ordering = o;
    }

    // Creates an evmdd for a constant term
    Evmdd<T> make_const_evmdd(T weight);

    // Creates an evmdd for a variable (i.e. one node representing the variable,
    // connected to the terminal node)
    Evmdd<T> make_var_evmdd(std::string const &var,
                            std::vector<T> const &domain);

    // Creates a new evmdd representing the term 'left oper right'
    template <typename F>
    Evmdd<T> apply(Evmdd<T> const &left, Evmdd<T> const &right, F oper) {
        if (terminal_case(left, right, oper)) {
            return make_terminal_evmdd(left, right, oper);
        }
        // TODO implement caching for apply
        std::vector<Evmdd<T>> left_sub_evmdds = sub_evmdds(left, right);
        std::vector<Evmdd<T>> right_sub_evmdds = sub_evmdds(right, left);
        assert(left_sub_evmdds.size() > 0);
        assert(left_sub_evmdds.size() == right_sub_evmdds.size());
        int root_level;
        std::string var;
        if (left.entry_node.get_level() <= right.entry_node.get_level()) {
            root_level = right.entry_node.get_level();
            var = right.entry_node.get_variable();
        } else {
            root_level = left.entry_node.get_level();
            var = left.entry_node.get_variable();
        }

        std::vector<Evmdd<T>> new_children;
        for (size_t i = 0; i < left_sub_evmdds.size(); ++i) {
            new_children.push_back(
                apply(left_sub_evmdds[i], right_sub_evmdds[i], oper));
        }
        return create_evmdd(root_level, var, new_children);
    }

private:
    // Returns true if the operation can immmediately be computed between the
    // evmdds.
    template <typename F>
    bool terminal_case(Evmdd<T> const &left, Evmdd<T> const &right,
                       F /*oper*/) {
        return (left.entry_node.get_level() == 0 &&
                right.entry_node.get_level() == 0);
    }

    // computation of 'left oper right' if it is a terminal operation
    template <typename F>
    Evmdd<T> make_terminal_evmdd(Evmdd<T> const &left, Evmdd<T> const &right,
                                 F oper) {
        T input_value = oper(left.input_value, right.input_value);
        return make_const_evmdd(input_value);
    }

    // If level of f is smaller than level of g, copy f |g.children| times.
    // Otherwise create an evmdd for each child of f with (weight+input value)
    // as new input value and the child as root node.
    std::vector<Evmdd<T>> sub_evmdds(Evmdd<T> const &f, Evmdd<T> const &g) {
        std::vector<Evmdd<T>> result;
        if (f.entry_node.get_level() >= g.entry_node.get_level()) {
            for (Edge<T> const &edge : f.entry_node.get_children()) {
                T input = f.input_value + edge.first.expression;
                Node<T> root_node = node_factory.get_node(edge.second);
                Evmdd<T> evmdd(input, root_node);
                result.push_back(evmdd);
            }
        } else {
            for (size_t i = 0; i < g.entry_node.get_children().size(); ++i) {
                result.push_back(f);
            }
        }
        return result;
    }

    // Returns a evmdd with root node at level, with input value as the minimal
    // input value of all children. Weights to each child is its original
    // input value minus the minimal input value.
    Evmdd<T> create_evmdd(int level, std::string var,
                          std::vector<Evmdd<T>> const &children) {
        Evmdd<T> min_weight_evmdd =
            *std::min_element(children.begin(), children.end(),
                              [](Evmdd<T> const &e1, Evmdd<T> const &e2) {
                                  return e1.input_value < e2.input_value;
                              });
        T min_weight = min_weight_evmdd.input_value;
        std::vector<Edge<T>> edges;
        for (Evmdd<T> const &child : children) {
            edges.emplace_back(Label<T>{child.input_value - min_weight},
                               child.entry_node.get_id());
        }
        Node<T> root_node = node_factory.make_node(level, var, edges);
        return Evmdd<T>(min_weight, root_node);
    }

    // variable ordering
    Ordering ordering;
    NodeFactory<T> node_factory;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
