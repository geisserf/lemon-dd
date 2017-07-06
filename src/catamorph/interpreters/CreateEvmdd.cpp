#include "CreateEvmdd.h"
#include "../Catamorph.h"
#include "../EvmddExpression.h"

template <>
Evmdd<EvmddExpression<float>> CreateEvmdd<EvmddExpression<float>>::makeVarEvmdd(
    const std::string var, unsigned int domain) {
    // terminal node
    Node<EvmddExpression<float>> terminal;

    // Incoming Edge
    Label<EvmddExpression<float>> incoming_label;
    EvmddExpression<float> expression;
    expression.value = 0;
    incoming_label.expression = expression;
    Edge<EvmddExpression<float>> incoming_edge;
    incoming_edge.label = incoming_label;
    incoming_edge.value = 0;

    // Variable Node
    Node<EvmddExpression<float>> variable_node;
    variable_node.variable = var;
    variable_node.incoming.push_back(incoming_edge);

    // for each domain value add edge from incoming_edge.get()->successor to
    // terminal with weight = domain value
    for (unsigned int d = 0; d < domain; d++) {
        Label<EvmddExpression<float>> edge_label;
        EvmddExpression<float> edge_expression;
        edge_expression.value = d;
        edge_label.expression = edge_expression;
        Edge<EvmddExpression<float>> edge;
        edge.label = edge_label;
        edge.value = d;
        edge.successor = terminal;
        edge.predecessor = variable_node;
        variable_node.outgoing.push_back(edge);
    }

    incoming_edge.successor = variable_node;
    Evmdd<EvmddExpression<float>> result;
    result.evmdd = incoming_edge;
    result.terminal = terminal;

    return result;
}

template <>
Evmdd<EvmddExpression<float>>
CreateEvmdd<EvmddExpression<float>>::makeConstEvmdd(const float weight) {
    Edge<EvmddExpression<float>> edge;
    Label<EvmddExpression<float>> l;
    EvmddExpression<float> expression;
    expression.value = weight;
    l.expression = std::move(expression);
    edge.label = std::move(l);
    edge.value = 0;

    Node<EvmddExpression<float>> successor;
    edge.successor = successor;
    Evmdd<EvmddExpression<float>> result;
    result.terminal = successor;
    result.evmdd = edge;
    return result;
}

template <>
auto CreateEvmdd<EvmddExpression<float>>::create_evmdd_alg(
    Domains const &domains) {
    return
        [&domains, this](expression_r<Evmdd<EvmddExpression<float>>> const &e)
            -> Evmdd<EvmddExpression<float>> {
                if (auto *o = Factories::get_as_cst(e)) {
                    return makeConstEvmdd(*o);
                }
                if (auto *o = Factories::get_as_var(e)) {
                    return makeVarEvmdd(*o, domains.find(*o)->second);
                }
                // not implemented :-)
                return makeConstEvmdd(0);
            };
}

template <>
Evmdd<EvmddExpression<float>> CreateEvmdd<EvmddExpression<float>>::create_evmdd(
    Expression const &e, Domains const &domains) {
    return Catamorph::cata<Evmdd<EvmddExpression<float>>>(
        [&domains, this](expression_r<Evmdd<EvmddExpression<float>>> const &e_)
            -> Evmdd<EvmddExpression<float>> {
                return create_evmdd_alg(domains)(e_);
            },
        e);
}

template class CreateEvmdd<EvmddExpression<float>>;
