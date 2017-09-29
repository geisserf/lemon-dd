#ifndef NODE_H
#define NODE_H

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

using PartialState = std::map<std::string, std::vector<int>>;

template <typename T>
class NodeFactory;

template <typename T>
class Node {
private:
    Node(int id, unsigned int level, std::string const &var,
         std::vector<Edge<T>> const &children);

    int id;
    unsigned int level;
    std::string variable;
    std::vector<Edge<T>> children;

    friend NodeFactory<T>;
    friend NodeStorage<T>;

public:
    void print(std::ostream &out, std::string indent = "") const;

    // Collect all successor nodes not yet evaluated in succ
    void unique_successor_nodes(std::unordered_set<Node_ptr<T>> &succ) const;

    int get_id() const {
        return id;
    }

    std::vector<Edge<T>> const &get_children() const {
        return children;
    }

    std::string const &get_variable() const {
        return variable;
    }

    unsigned int get_level() const {
        return level;
    }

    bool is_terminal() const {
        return id == 0;
    }

    template <typename Res, typename EvaluationFunction>
    Res evaluate(PartialState const &state, EvaluationFunction func) const {
        // Terminal node backpropagates neutral element
        if (is_terminal()) {
            return Res();
        }

        std::vector<Res> child_results;
        auto state_it = state.find(variable);
        if (state_it != state.end()) {
            // Only compute for domain values contained in the partial state
            for (int d_val : state_it->second) {
                assert(static_cast<size_t>(d_val) < children.size());
                Res child_res =
                    children[d_val].second->evaluate<Res>(state, func);
                child_res += children[d_val].first;
                child_results.push_back(child_res);
            }
        } else {
            // TODO This is basically the same code as above, just for the case
            // that the whole domain is covered. It would be nice if we could
            // remove this code duplication.
            for (size_t d_val = 0; d_val < children.size(); ++d_val) {
                Res child_res =
                    children[d_val].second->evaluate<Res>(state, func);
                child_res += children[d_val].first;
                child_results.push_back(child_res);
            }
        }
        return func(child_results);
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
    Node_ptr<T> make_node(unsigned int level, std::string const &variable,
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
