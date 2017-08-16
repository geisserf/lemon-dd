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

using Ordering = std::map<std::string, int>;

template <typename T>
class Node;

template <typename T>
class EvmddFactory;

template <typename T>
class Evmdd {
private:
    T input_value;
    std::shared_ptr<Node<T> const> entry_node;

    Evmdd(T input, std::shared_ptr<Node<T> const> entry_node)
        : input_value(input), entry_node(entry_node) {}
    template <typename R>
    friend class EvmddFactory;

public:
    void print(std::ostream &out) {
        out << "input value: " << input_value.toString();
        out << " nodes:" << std::endl;
        entry_node->print(out);
    }
    std::vector<T> evaluate_partial(
        std::map<std::string, std::vector<int>> const &state) {
        return evaluate(state, greates_lower_bound<T>());
    }

    template <typename EvaluationFunction>
    std::vector<T> evaluate(State const &state,
                            EvaluationFunction eval_function) {
        std::vector<T> per_state_result =
            entry_node->evaluate(state, eval_function);
        std::vector<T> result;
        for (T res : per_state_result) {
            T current = res + input_value;
            result = eval_function(current, result);
        }
        return result;
    }

    std::vector<T> get_min() {
        return evaluate(std::map<std::string, std::vector<int>>(),
                        greates_lower_bound<T>());
    }

    std::vector<T> get_max() {
        return evaluate(std::map<std::string, std::vector<int>>(),
                        least_upper_bound<T>());
    }

    std::shared_ptr<Node<T> const> const &get_entry_node() const {
        assert(entry_node);
        return entry_node;
    }

    T get_input_value() const {
        return input_value;
    }

    // Returns the number of nodes
    int size() const {
        std::unordered_set<Node_ptr<T>> succ;
        entry_node->unique_successor_nodes(succ);
        // +1 counts entry node
        return succ.size() + 1;
    }
};

template <typename T>
class EvmddFactory {
public:
    // TODO we should not be able to change the ordering for the factory, as
    // this messes up the correspondence between levels and variables in the
    // node storage.
    void set_ordering(Ordering const &o) {
        ordering = o;
    }

    // Creates an evmdd for a constant term
    Evmdd<T> make_const_evmdd(T weight) {
        return Evmdd<T>(weight, node_factory.get_terminal_node());
    }
    // Creates an evmdd for a variable (i.e. one node representing the variable,
    // connected to the terminal node)
    Evmdd<T> make_var_evmdd(std::string const &var,
                            std::vector<T> const &domain) {
        std::vector<Edge<T>> children;
        for (size_t i = 0; i < domain.size(); ++i) {
            children.emplace_back(T(domain[i]),
                                  node_factory.get_terminal_node());
        }
        std::shared_ptr<Node<T> const> node =
            node_factory.make_node(ordering[var], var, children);
        return Evmdd<T>(T::identity(), node);
    }

    // Creates a new evmdd representing the term 'left oper right'
    template <typename L, typename R, typename F>
    Evmdd<T> apply(Evmdd<L> const &left, Evmdd<R> const &right, F oper) {
        if (terminal_case(left, right, oper)) {
            return make_terminal_evmdd(left, right, oper);
        }
        // TODO implement caching for apply
        std::vector<Evmdd<L>> left_sub_evmdds = sub_evmdds(left, right);
        std::vector<Evmdd<R>> right_sub_evmdds = sub_evmdds(right, left);
        assert(left_sub_evmdds.size() > 0);
        assert(left_sub_evmdds.size() == right_sub_evmdds.size());

        // TODO variable should be retrieved by the ordering and not stored in
        // nodes
        int root_level;
        std::string var;
        if (left.get_entry_node()->get_level() <=
            right.get_entry_node()->get_level()) {
            root_level = right.entry_node->get_level();
            var = right.entry_node->get_variable();
        } else {
            root_level = left.get_entry_node()->get_level();
            var = left.get_entry_node()->get_variable();
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
    template <typename F, typename L, typename R>
    bool terminal_case(Evmdd<L> const &left, Evmdd<R> const &right,
                       F /*oper*/) {
        // TODO use oper to determine earlier terminal case
        return (left.get_entry_node()->is_terminal() &&
                right.get_entry_node()->is_terminal());
    }

    // computation of 'left oper right' if it is a terminal operation
    template <typename F, typename L, typename R>
    Evmdd<T> make_terminal_evmdd(Evmdd<L> const &left, Evmdd<R> const &right,
                                 F oper) {
        T input_value = oper(left.get_input_value(), right.get_input_value());
        return make_const_evmdd(input_value);
    }

    // If level of f is smaller than level of g, copy f |g.children| times.
    // Otherwise create an evmdd for each child of f with (weight+input value)
    // as new input value and the child as root node.
    template <typename L, typename R>
    std::vector<Evmdd<L>> sub_evmdds(Evmdd<L> const &f, Evmdd<R> const &g) {
        std::vector<Evmdd<L>> result;
        if (f.get_entry_node()->get_level() >=
            g.get_entry_node()->get_level()) {
            for (Edge<L> const &edge : f.get_entry_node()->get_children()) {
                L input = f.get_input_value() + edge.first;
                Evmdd<L> evmdd(input, edge.second);
                result.push_back(evmdd);
            }
        } else {
            for (size_t i = 0; i < g.get_entry_node()->get_children().size();
                 ++i) {
                result.push_back(f);
            }
        }
        return result;
    }
    // returns the greatest lower bound from the input_values
    T greatest_lower_bound(std::vector<Evmdd<T>> const &children);

    // Returns an evmdd with root node at level, with input value as the minimal
    // input value of all children. Weights to each child is its original
    // input value minus the minimal input value.
    Evmdd<T> create_evmdd(int level, std::string var,
                          std::vector<Evmdd<T>> const &children) {
        T min_weight = greatest_lower_bound(children);
        std::vector<Edge<T>> edges;
        for (Evmdd<T> const &child : children) {
            edges.emplace_back(T{child.get_input_value() - min_weight},
                               child.get_entry_node());
        }
        Node_ptr<T> root_node = node_factory.make_node(level, var, edges);
        return Evmdd<T>(min_weight, root_node);
    }

    // variable ordering
    Ordering ordering;
    NodeFactory<T> node_factory;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
