#ifndef NODE_H
#define NODE_H

#include "node_factory.h"

#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <vector>

using PartialState = std::map<std::string, std::vector<int>>;
template <typename T>
class Node;

template <typename EvaluationFunction, typename Res, typename T>
using EvalCacheMap = std::map<Node<T>, Res>;

template <typename EvaluationFunction, typename Res, typename T>
class EvalCache {
public:
    static typename EvalCacheMap<EvaluationFunction, Res, T>::iterator find(
        Node<T> const &node) {
        return cache.find(node);
    }

    static void add(Node<T> const &node, Res const &res) {
        cache[node] = res;
    }

    static typename EvalCacheMap<EvaluationFunction, Res, T>::iterator end() {
        return cache.end();
    }
    static void clear() {
        cache.clear();
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
        auto found = EvalCache<EvaluationFunction, Res, T>::find(*this);
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
        EvalCache<EvaluationFunction, Res, T>::add(*this, res);
        return res;
    }

    friend bool operator<(const Node<T> &l, const Node<T> &r) {
        return l.get_id() < r.get_id();
    }
};

#endif /* NODE_H */
