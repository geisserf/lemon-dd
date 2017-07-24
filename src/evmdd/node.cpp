#include "node.h"
#include "evmdd_expression.h"

template <typename T>
void Node<T>::print(std::ostream &out, std::string indent) const {
    out << indent << "ID: " << id << std::endl;
    indent += "  ";
    for (size_t i = 0; i < children.size(); ++i) {
        out << indent << "w[" << i
            << "]: " << children[i].first.expression.toString() << std::endl;
        //    << " c[i]: "<<std::endl;
        storage->get_node(children[i].second).print(out, indent + "  ");
    }
}

template <typename T>
Node<T>::Node(int id, int level, std::string const &var,
              std::vector<Edge<T>> const &children, NodeStorage<T> *storage)
    : id(id), level(level), variable(var), children(children),
      storage(storage) {}

template <typename T>
NodeFactory<T>::NodeFactory()
    : storage(NodeStorage<T>()), node_counter(storage.size()) {}

template <typename T>
Node<T> NodeFactory<T>::get_terminal_node() {
    return storage.get_node(0);
}

template <typename T>
Node<T> NodeFactory<T>::make_node(int level, std::string const &variable,
                                  std::vector<Edge<T>> const &children) {
    // if (storage.find(level, variable, children) == storage.end)...
    Node<T> node(node_counter++, level, variable, children, &storage);
    storage.add_node(node);
    return node;
}

template class Node<NumericExpression>;
template class Node<VariableAssignmentExpression>;
template class Node<TupleExpression>;

template class NodeFactory<NumericExpression>;
template class NodeFactory<VariableAssignmentExpression>;
template class NodeFactory<TupleExpression>;
