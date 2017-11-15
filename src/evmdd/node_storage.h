#ifndef NODE_STORAGE_H
#define NODE_STORAGE_H

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
class NodeStorage {
public:
    // ~NodeStorage() {
    //     int collisions = 0;
    //     for (size_t i = 0; i < id_cache.size(); ++i) {
    //         if (id_cache.bucket_size(i) > 1) {
    //             ++collisions;
    //         }
    //     }
    //     std::cout << "Nodes: " << lookup.size() << std::endl;
    //     std::cout << "Collisions: " << collisions << std::endl;
    // }

    Node_ptr<T> get_node(int id) const {
        return lookup.at(id);
    }

    Node_ptr<T> exists(int level, std::vector<Edge<T>> const &edges) {
        Sorting_key key = hash_value(level, edges);
        return id_cache[key];
    }

    void add_node(Node_ptr<T> node) {
        lookup.insert(std::make_pair<>(node->get_id(), node));
        Sorting_key key = hash_value(node->get_level(), node->get_children());
        assert(id_cache[key] == nullptr);
        id_cache[key] = node;
    }

    size_t size() const {
        return lookup.size();
    }

    void print_nodes(std::ostream& out) const {
        out << "Nodes stored: " << std::endl;
        for (auto const& kv : lookup) {
            out << kv.second->to_string() << std::endl;
        }
    }

    // Automatically constructs the terminal node on initialization
    NodeStorage() : lookup({{0, Node_ptr<T>(new Node<T>(0, 0, " ", {}))}}) {}

private:
    // Storage for nodes, in case no evmdd uses a node, but we may want to use
    // them again for further evmdd construction
    std::unordered_map<int, Node_ptr<T>> lookup;

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

#endif /* NODE_STORAGE_H */
