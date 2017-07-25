#ifndef NODE_STORAGE_H
#define NODE_STORAGE_H

#include <cassert>
#include <iostream>
#include <map>
#include <memory>

// Storage class to manage evmdd nodes.

template <typename T>
class Node;

template <typename T>
using Node_ptr = std::shared_ptr<Node<T> const>;

template <typename T>
using Edge = std::pair<T, Node_ptr<T>>;

template <typename T>
class NodeStorage {
public:
    Node_ptr<T> get_node(int id) const {
        return lookup.at(id);
    }

    Node_ptr<T> exists(int id, std::vector<Edge<T>> const &children) {
        Sorting_key key(id, children);
        return id_cache[key];
    }

    void add_node(Node_ptr<T> node) {
        lookup.insert(std::make_pair<>(node->get_id(), node));
        Sorting_key key(node->get_level(), node->get_children());
        if (id_cache[key]) id_cache[key]->print(std::cout, "");
        assert(id_cache[key] == nullptr);
        id_cache[key] = node;
    }

    size_t size() const {
        return lookup.size();
    }

    // Automatically constructs the terminal node on initialization
    NodeStorage() : lookup({{0, Node_ptr<T>(new Node<T>(0, 0, " ", {}))}}) {}

private:
    // Storage for nodes, in case no evmdd uses a node, but we may want to use
    // them again for further evmdd construction
    std::map<int, Node_ptr<T>> lookup;

    // For each new node there is an entry (level,weights,children)->node
    using Sorting_key = std::tuple<int, std::vector<Edge<T>>>;
    std::map<Sorting_key, Node_ptr<T>> id_cache;
};

#endif /* NODE_STORAGE_H */
