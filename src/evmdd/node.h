#ifndef NODE_H
#define NODE_H

#include "node_storage.h"

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

template <typename T>
class NodeFactory;

template <typename T>
class Node {
private:
    Node() {}
    Node(int id, int level, std::string const &var,
         std::vector<Edge<T>> const &children);

    int id;
    int level;
    std::string variable;
    std::vector<Edge<T>> children;

    friend NodeFactory<T>;
    friend NodeStorage<T>;
    friend std::shared_ptr<Node<T> const>;

public:
    void print(std::ostream &out) const {
        out << "ID: " << id << std::endl;
        for (size_t i = 0; i < children.size(); ++i) {
            out << "\tw[i]: " << children[i].first.expression.toString()
                << " c[i]: " << children[i].second->get_id() << std::endl;
        }
    }

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
    NodeStorage<T> storage;
    int node_counter;

    // Retrieves the node with the given id
    std::shared_ptr<Node<T> const> get_node(int id) const {
        return storage.get_node(id);
    }
};

#endif /* NODE_H */
