#include "CreateEvmdd.h"
#include "../Catamorph.h"
#include "../EvmddExpression.h"

template <>
Evmdd<EvmddExpression<float>>
CreateEvmdd<EvmddExpression<float>>::makeConstEvmdd(const float *weight) {
    auto edge = std::shared_ptr<Edge<EvmddExpression<float>>>(
        new Edge<EvmddExpression<float>>());
    auto l = std::unique_ptr<Label<EvmddExpression<float>>>(
        new Label<EvmddExpression<float>>());
    auto expression =
        std::unique_ptr<NumericExpression>(new EvmddExpression<float>());
    expression.get()->value = *weight;
    l.get()->expression = std::move(expression);
    edge.get()->label = std::move(l);

    edge.get()->successor = std::shared_ptr<Node<EvmddExpression<float>>>(
        new Node<EvmddExpression<float>>());
    Evmdd<EvmddExpression<float>>
        result; // = new Evmdd<EvmddExpression<float>>();
    result.terminal = edge.get()->successor;
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
                    return makeConstEvmdd(o);
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
