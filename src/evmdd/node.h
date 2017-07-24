#ifndef NODE_H
#define NODE_H

#include "evmdd_expression.h"
#include "node_storage.h"

#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>

template <typename T>
class Label {
public:
    Label(T expr) : expression(expr) {}
    T expression;
};

template <typename T>
class Node;

template <typename T>
using Edge = std::pair<Label<T>, std::shared_ptr<Node<T> const>>;
using State = std::map<std::string, std::vector<int>>;

template <typename T>
class NodeFactory;

template <typename T>
class Node {
private:
    Node(int id, int level, std::string const &var,
         std::vector<Edge<T>> const &children);

    int id;
    int level;
    std::string variable;
    std::vector<Edge<T>> children;

    friend NodeFactory<T>;
    friend NodeStorage<T>;

public:
    void print(std::ostream &out, std::string indent = "") const;

    int get_id() const {
        return id;
    }

    std::vector<Edge<T>> get_children() const {
        return children;
    }

    std::string get_variable() const {
        return variable;
    }

    int get_level() const {
        return level;
    }

    bool is_terminal() const {
        return id == 0;
    }

    template <typename EvaluationFunction>
    std::vector<T> evaluate(State const &state, EvaluationFunction func) const {
        std::vector<T> result;
        auto state_values_iter = state.find(variable);
        // state does not permit every domain value for this variable
        if (state_values_iter != state.end()) {
            std::vector<int> state_values = state_values_iter->second;
            for (size_t i = 0; i < state_values.size(); ++i) {
                assert(static_cast<size_t>(state_values[i]) < children.size());
                if (children[state_values[i]].second->is_terminal()) {
                    result = func(children[state_values[i]].first.expression,
                                  result);
                } else {
                    std::vector<T> child_result =
                        children[state_values[i]].second->evaluate(state, func);
                    T const &weight =
                        children[state_values[i]].first.expression;

                    // add weight to child evaluations:
                    for (T child_expr : child_result) {
                        result = func(child_expr + weight, result);
                    }
                }
            }
        } else {
            // state contains all domain values for this variable
            for (size_t i = 0; i < children.size(); ++i) {
                if (children[i].second->is_terminal()) {
                    result = func(children[i].first.expression, result);
                } else {
                    // get evaluation of child
                    std::vector<T> child_result =
                        children[i].second->evaluate(state, func);
                    T const &weight = children[i].first.expression;
                    // add weight to child evaluations:
                    for (T child_expr : child_result) {
                        result = func(child_expr + weight, result);
                    }
                }
            }
        }
        return result;
    }
};

template <typename T>
class NodeFactory {
public:
    NodeFactory();

    // Returns a pointer to the (unique) terminal node
    std::shared_ptr<Node<T> const> get_terminal_node() const;

    // Returns a pointer to the given node. If the node is not yet stored, it is
    // created first.
    std::shared_ptr<Node<T> const> make_node(
        int level, std::string const &variable,
        std::vector<Edge<T>> const &children);

private:
    // Manages node memory
    NodeStorage<T> storage;

    // How many nodes this factory created
    int node_counter;

    // Retrieves the node with the given id
    std::shared_ptr<Node<T> const> get_node(int id) const {
        return storage.get_node(id);
    }
};

#endif /* NODE_H */
