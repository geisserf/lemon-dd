#include "evmdd.h"
#include "evmdd_expression.h"
#include <iostream>
#include <queue>

using std::vector;
using std::string;
using std::shared_ptr;

template <typename T>
Evmdd<T>::Evmdd(T input, shared_ptr<Node<T> const> entry_node)
    : input_value(input), entry_node(entry_node) {}

template <typename T>
Evmdd<T> EvmddFactory<T>::make_const_evmdd(T weight) {
    return Evmdd<T>(weight, node_factory.get_terminal_node());
}

template <typename T>
Evmdd<T> EvmddFactory<T>::make_var_evmdd(string const &var,
                                         vector<T> const &domain) {
    vector<Edge<T>> children;
    for (size_t i = 0; i < domain.size(); ++i) {
        children.emplace_back(Label<T>(domain[i]),
                              node_factory.get_terminal_node());
    }
    shared_ptr<Node<T> const> node =
        node_factory.make_node(ordering[var], var, children);
    return Evmdd<T>(T::identity(), node);
}

template class Evmdd<NumericExpression>;
template class Evmdd<VariableAssignmentExpression>;
template class Evmdd<TupleExpression>;

template class EvmddFactory<NumericExpression>;
template class EvmddFactory<VariableAssignmentExpression>;
template class EvmddFactory<TupleExpression>;
