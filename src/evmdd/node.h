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
using Edge = std::pair<Label<T>, int>;

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

public:
    void print(std::ostream &out) const {
        out << "IMPLEMENT ME";
    }

    int get_id() const {
        return id;
    }
    std::vector<Edge<T>> get_children() {
        return children;
    }
};

template <typename T>
class NodeFactory {
public:
    NodeFactory();
    Node<T> get_terminal_node();
    Node<T> get_node(int id) {
        return storage.get_node(id);
    }
    Node<T> make_node(int level, std::string const &variable,
                      std::vector<Edge<T>> const &children);

private:
    int node_counter;
    NodeStorage<T> storage;
};

#endif /* NODE_H */
