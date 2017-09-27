#include "node.h"
#include "monoid.h"

template <typename T>
void Node<T>::print(std::ostream &out, std::string indent) const {
    out << indent << "ID: " << id << "(" << variable << ")" << std::endl;
    indent += "  ";
    for (size_t i = 0; i < children.size(); ++i) {
        out << indent << "w[" << i << "]: " << children[i].first.to_string()
            << std::endl;
        children[i].second->print(out, indent + "  ");
    }
}

template <typename T>
void Node<T>::unique_successor_nodes(
    std::unordered_set<Node_ptr<T>> &successors) const {
    for (auto const &edge : children) {
        // Only evaluate children which were not yet evaluated
        if (successors.find(edge.second) == successors.end()) {
            successors.insert(edge.second);
            edge.second->unique_successor_nodes(successors);
        }
    }
}

template <typename T>
Node<T>::Node(int id, int level, std::string const &var,
              std::vector<Edge<T>> const &children)
    : id(id), level(level), variable(var), children(children) {}

template <typename T>
NodeFactory<T>::NodeFactory()
    : storage(NodeStorage<T>()), node_counter(storage.size()) {}

template <typename T>
Node_ptr<T> NodeFactory<T>::get_terminal_node() const {
    return storage.get_node(0);
}

template <typename T>
Node_ptr<T> NodeFactory<T>::make_node(int level, std::string const &variable,
                                      std::vector<Edge<T>> const &children) {
    assert(level != 0);
    if (auto cached = storage.exists(level, children)) {
        return cached;
    }
    Node_ptr<T> node(new Node<T>(node_counter++, level, variable, children));
    storage.add_node(node);
    return node;
}

template class Node<Monoid<float>>;
template class NodeFactory<Monoid<float>>;
template class Node<Monoid<double>>;
template class NodeFactory<Monoid<double>>;
template class Node<Monoid<int>>;
template class NodeFactory<Monoid<int>>;
