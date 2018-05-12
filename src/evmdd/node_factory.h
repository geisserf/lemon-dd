#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H

#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>

#include <boost/functional/hash.hpp>

// Storage class to manage evmdd nodes.

template <typename T>
class Node;

template <typename T>
using Node_ptr = std::shared_ptr<Node<T> const>;

template <typename T>
using Edge = std::pair<T, Node_ptr<T>>;

using Sorting_key = size_t;

template <typename T>
class NodeFactory {
public:
    // Returns a pointer to the (unique) terminal node
    Node_ptr<T> get_terminal_node() const {
        return id_cache.at(0);
    }

    // Returns a pointer to the given node. If the node is not yet stored, it is
    // created first.
    Node_ptr<T> make_node(unsigned int level, std::string const &variable,
                          std::vector<Edge<T>> const &children) {
        Sorting_key key = hash_value(level, children);
        assert(key != 0); // hash key 0 should only belong to the terminal node
        if (id_cache[key]) {
            return id_cache[key];
        }
        Node_ptr<T> node(new Node<T>(size(), level, variable, children));
        id_cache[key] = node;
        return node;
    }

    size_t size() const {
        return id_cache.size();
    }

    void print_nodes(std::ostream &out) const {
        out << size() << " nodes stored: " << std::endl;
        for (auto const &kv : id_cache) {
            out << kv.second->to_string() << std::endl;
        }
    }

    // Automatically constructs the terminal node on initialization
    NodeFactory() : id_cache({{0, Node_ptr<T>(new Node<T>(0, 0, " ", {}))}}) {}

private:
    // For each new node there is an entry (level,weights,children)->node
    std::unordered_map<Sorting_key, Node_ptr<T>> id_cache;

    // Calculates the hash value for a node based on its level and edges.
    // This implies that every edge type T has to implement hash_value.
    Sorting_key hash_value(int level, std::vector<Edge<T>> const &edges) {
        size_t hash = boost::hash_range(edges.begin(), edges.end());
        size_t seed = 0;
        boost::hash_combine(seed, level);
        boost::hash_combine(seed, hash);
        return seed;
    }
};

#endif /* NODE_FACTORY_H */
