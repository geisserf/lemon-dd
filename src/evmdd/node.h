#ifndef NODE_H
#define NODE_H

#include "node_storage.h"

#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

using PartialState = std::map<std::string, std::vector<int>>;
template <typename T>
class Node;

// template <typename T>
// struct NodeCompare {
//    bool operator()(const std::pair<Node<T>, PartialState> &lhs,
//                    const std::pair<Node<T>, PartialState> &rhs) const {
//        return lhs.first.get_id() < rhs.first.get_id();
//    }
//};

template <typename EvaluationFunction, typename Res, typename T>
using EvalCacheMap = std::map<std::pair<Node<T>, PartialState>, Res>;

template <typename EvaluationFunction, typename Res, typename T>
class EvalCache {
public:
    static typename EvalCacheMap<EvaluationFunction, Res, T>::iterator find(
        PartialState const &state, Node<T> const &node) {
        return cache.find(std::pair<Node<T>, PartialState>(node, state));
    }

    static void add(Node<T> const &node, Res const &res,
                    PartialState const &state) {
        cache[std::pair<Node<T>, PartialState>(node, state)] = res;
    }

    static typename EvalCacheMap<EvaluationFunction, Res, T>::iterator end() {
        return cache.end();
    }

private:
    static EvalCacheMap<EvaluationFunction, Res, T> cache;
};

template <typename EvaluationFunction, typename Res, typename T>
EvalCacheMap<EvaluationFunction, Res, T>
    EvalCache<EvaluationFunction, Res, T>::cache =
        EvalCacheMap<EvaluationFunction, Res, T>();

template <typename T>
using Edge = std::pair<T, Node_ptr<T>>;

template <typename T>
class NodeFactory;

template <typename T>
class Node {
private:
    Node(int id, unsigned int level, std::string const &var,
         std::vector<Edge<T>> const &children)
        : id(id), level(level), variable(var), children(children) {}

    int id;
    unsigned int level;
    std::string variable;
    std::vector<Edge<T>> children;

    friend NodeFactory<T>;
    friend NodeStorage<T>;

public:
    // returns "var [w1] ... [w_n]"
    // At some point we may want to indicate which node is connected to which
    // node, but the current structure is not really sufficient for that.
    std::string to_string() const {
        std::string result = variable;
        for (auto const &edge : children) {
            result += " " + edge.first.to_string();
        }
        return result;
    }

    // Collect all successor nodes not yet evaluated in succ
    void unique_successor_nodes(std::unordered_set<Node_ptr<T>> &succ) const {
        for (auto const &edge : children) {
            // Only evaluate children which were not yet evaluated
            if (succ.find(edge.second) == succ.end()) {
                succ.insert(edge.second);
                edge.second->unique_successor_nodes(succ);
            }
        }
    }

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

    unsigned int get_depth() const {
        if (is_terminal()) {
            return 0;
        }
        return children[0].second->get_depth() + 1;
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
        auto found = EvalCache<EvaluationFunction, Res, T>::find(state, *this);
        if (found != EvalCache<EvaluationFunction, Res, T>::end()) {
            return found->second;
        }

        std::vector<Res> child_results;
        auto state_it = state.find(variable);
        if (state_it != state.end()) {
            // Only compute for domain values contained in the partial state
            for (int d_val : state_it->second) {
                assert(static_cast<size_t>(d_val) < children.size());
                Res child_res =
                    children[d_val].second->template evaluate<Res>(state, func);
                child_res += children[d_val].first;
                child_results.push_back(child_res);
            }
        } else {
            // TODO This is basically the same code as above, just for the case
            // that the whole domain is covered. It would be nice if we could
            // remove this code duplication.
            for (size_t d_val = 0; d_val < children.size(); ++d_val) {
                Res child_res =
                    children[d_val].second->template evaluate<Res>(state, func);
                child_res += children[d_val].first;
                child_results.push_back(child_res);
            }
        }
        auto res = func(child_results);
        EvalCache<EvaluationFunction, Res, T>::add(*this, res, state);
        return res;
    }

    friend bool operator<(const Node<T> &l, const Node<T> &r) {
        return l.get_id() < r.get_id();
    }
};

template <typename T>
class NodeFactory {
public:
    NodeFactory() : storage(NodeStorage<T>()), node_counter(storage.size()) {}

    size_t size() const {
        return storage.size();
    }

    // Returns a pointer to the (unique) terminal node
    Node_ptr<T> get_terminal_node() const {
        return storage.get_node(0);
    }

    // Returns a pointer to the given node. If the node is not yet stored, it is
    // created first.
    Node_ptr<T> make_node(unsigned int level, std::string const &variable,
                          std::vector<Edge<T>> const &children) {
        assert(level != 0);
        if (auto cached = storage.exists(level, children)) {
            return cached;
        }
        Node_ptr<T> node(
            new Node<T>(node_counter++, level, variable, children));
        storage.add_node(node);
        return node;
    }

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
