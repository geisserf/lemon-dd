#ifndef NODE_STORAGE_H
#define NODE_STORAGE_H

#include <map>

// Storage class to manage evmdd nodes.

template <typename T>
class Node;

template <typename T>
class NodeStorage {
public:
    // NodeStorage(Node<T> const &terminal_node) : lookup({{0, terminal_node}})
    // {}

    const Node<T> &get_node(int id) {
        return lookup.at(id);
    }

    void add_node(Node<T> const &node) {
        lookup.insert(std::make_pair<>(node.get_id(), node));
    }
    static NodeStorage &getInstance() {
        static NodeStorage instance;
        return instance;
    }

private:
    std::map<int, Node<T>> lookup;
    NodeStorage() : lookup({{0, Node<T>(0, 0, "", {})}}) {}
};

#endif /* NODE_STORAGE_H */
