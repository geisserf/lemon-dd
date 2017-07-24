#ifndef NODE_STORAGE_H
#define NODE_STORAGE_H

#include <iostream>
#include <map>
#include <memory>

// Storage class to manage evmdd nodes.

template <typename T>
class Node;

template <typename T>
class NodeStorage {
public:
    std::shared_ptr<Node<T> const> get_node(int id) const {
        return lookup.at(id);
    }

    void add_node(std::shared_ptr<Node<T> const> node) {
        lookup.insert(std::make_pair<>(node->get_id(), node));
    }
    // static NodeStorage &getInstance() {
    //     static NodeStorage instance;
    //     std::cout << "instance size on factory call " << instance.size() <<
    //     std::endl;
    //     return instance;
    // }

    size_t size() const {
        return lookup.size();
    }

    // Automatically constructs the terminal node on initialization
    NodeStorage()
        : lookup({{0, std::shared_ptr<Node<T> const>(
                          new Node<T>(0, 0, " ", {}))}}) {}

private:
    std::map<int, std::shared_ptr<Node<T> const>> lookup;
};

#endif /* NODE_STORAGE_H */
