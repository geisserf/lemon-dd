#ifndef NODE_H
#define NODE_H

#include "evmdd_expression.h"
#include "node_storage.h"

#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename T>
class Node;

template <typename T>
using Edge = std::pair<T, Node_ptr<T>>;
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

    // Collect all successor nodes not yet evaluated in succ
    void unique_successor_nodes(std::unordered_set<Node_ptr<T>> &succ) const;
    int num_successor_nodes() const;

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
        // Terminal node backpropagates identity element
        if (is_terminal()) {
            return std::vector<T>{T::identity()};
        }

        std::vector<T> result;
        auto state_it = state.find(variable);
        // state does not permit every domain value for this variable
        if (state_it != state.end()) {
            for (int domain_value : state_it->second) {
                assert(static_cast<size_t>(domain_value) < children.size());
                std::vector<T> child_result =
                    children[domain_value].second->evaluate(state, func);
                T const &weight = children[domain_value].first;

                // add weight to child evaluation:
                for (T child_expr : child_result) {
                    result = func(child_expr + weight, result);
                }
            }
        } else {
            // TODO This is basically the same code as above, just for the case
            // that the whole domain is covered. It would be nice if we could
            // remove this code duplication
            for (size_t domain_value = 0; domain_value < children.size();
                 ++domain_value) {
                std::vector<T> child_result =
                    children[domain_value].second->evaluate(state, func);
                T const &weight = children[domain_value].first;
                // add weight to child evaluations:
                for (T child_expr : child_result) {
                    result = func(child_expr + weight, result);
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
    Node_ptr<T> get_terminal_node() const;

    // Returns a pointer to the given node. If the node is not yet stored, it is
    // created first.
    Node_ptr<T> make_node(int level, std::string const &variable,
                          std::vector<Edge<T>> const &children);

private:
    // Manages node memory
    NodeStorage<T> storage;

    // How many nodes this factory created
    int node_counter;

    // Retrieves the node with the given id
    Node_ptr<T> get_node(int id) const {
        return storage.get_node(id);
    }
};

#endif /* NODE_H */
