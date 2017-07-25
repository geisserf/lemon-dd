#include "node.h"
#include "evmdd_expression.h"

using std::shared_ptr;

template <typename T>
void Node<T>::print(std::ostream &out, std::string indent) const {
    out << indent << "ID: " << id << std::endl;
    indent += "  ";
    for (size_t i = 0; i < children.size(); ++i) {
        out << indent << "w[" << i
            << "]: " << children[i].first.toString() << std::endl;
        children[i].second->print(out, indent + "  ");
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
shared_ptr<Node<T> const> NodeFactory<T>::get_terminal_node() const {
    return storage.get_node(0);
}

template <typename T>
shared_ptr<Node<T> const> NodeFactory<T>::make_node(
    int level, std::string const &variable,
    std::vector<Edge<T>> const &children) {
    if (auto cached = storage.exists(level, children)) {
        return cached;
    }
    shared_ptr<Node<T> const> node(
        new Node<T>(node_counter++, level, variable, children));
    storage.add_node(node);
    return node;
}

template class Node<NumericExpression>;
template class Node<VariableAssignmentExpression>;
template class Node<TupleExpression>;

template class NodeFactory<NumericExpression>;
template class NodeFactory<VariableAssignmentExpression>;
template class NodeFactory<TupleExpression>;
