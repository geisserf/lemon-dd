#include "node.h"
#include "evmdd_expression.h"

template <typename T>
Node<T>::Node(int id, int level, std::string const &var,
              std::vector<Edge<T>> const &children)
    : id(id), level(level), variable(var), children(children) {}

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
    Node<T> node(node_counter++, level, variable, children);
    storage.add_node(node);
    return node;
}

template class Node<NumericExpression>;
template class Node<VariableAssignmentExpression>;
template class Node<TupleExpression>;

template class NodeFactory<NumericExpression>;
template class NodeFactory<VariableAssignmentExpression>;
template class NodeFactory<TupleExpression>;

// template <typename T>
// void Evmdd<T>::print_rec(Edge<T> edge, std::string depth) {
//    std::cout << depth << "from: " << edge.predecessor.variable
//              << " with label " << edge.label.expression.toString()
//              << " to: " << edge.successor.variable << std::endl;
//    depth = depth + "  ";
//    for (auto child : edge.successor.outgoing) {
//        print_rec(child, depth);
//    }
//}
//
// template <typename T>
// Node<T> Node<T>::makeNode(std::vector<Edge<T>> outgoing, std::string
// variable,
//                          int level) {
//
//    auto element = Node<T>::memorized.find(hash_);
//
//    Node<T> node;
//    node.level = level;
//    node.variable = variable;
//    node.outgoing = outgoing;
//    for (auto it = node.outgoing.begin(); it != node.outgoing.end(); it++) {
//        it->predecessor = node;
//    }
//
//    Node<T>::memorized.insert(std::pair<size_t, Node<T>>(hash_, node));
//    // std::cout<<"Return new node: "<<variable<<std::endl;
//    return node;
//}
//
//
//
//
