#include "evmdd.h"

using std::map;
using std::vector;
using std::pair;

// stores cheapest cost we can reach from this edge and cheapest facts
template <typename T>
using EvalType = pair<map<Fact, T>, T>;

template <typename T>
using Weight = ProductMonoid<Facts, T, Union, std::plus<T>>;

template <typename T>
EvalType<T> operator+(EvalType<T> &result, Weight<T> const &weight) {
    map<Fact, T> &fact_costs = result.first;
    T edge_cost = weight.get_value().second;
    // Add edge cost to facts
    for (auto &kv : fact_costs) {
        kv.second += edge_cost;
    }
    // Add edge cost to current minimal cost
    result.second += edge_cost;
    // Add new facts to current facts
    for (Fact const &fact : weight.get_value().first) {
        assert(fact_costs.find(fact) == fact_costs.end());
        fact_costs[fact] = result.second;
    }
    return result;
}

// Definitely not the fastest implementation
template <typename T>
vector<pair<Fact, T>> Relaxation::evaluate(
    ProductEvmdd<Facts, T, Union, std::plus<T>> const &evmdd,
    PartialState const &state) {
    // Store only cheapest cost for each fact and cheapest overall cost
    auto eval = [](vector<EvalType<T>> const &child_results) {
        map<Fact, T> cheapest_facts;
        T cheapest_cost = std::numeric_limits<T>::max();
        for (EvalType<T> const &child_result : child_results) {
            cheapest_cost = std::min(cheapest_cost, child_result.second);
            // Store facts with their cheapest cost
            map<Fact, T> const &child_facts = child_result.first;
            for (auto const &kv : child_facts) {
                if (cheapest_facts.find(kv.first) == cheapest_facts.end()) {
                    cheapest_facts[kv.first] = kv.second;
                } else {
                    cheapest_facts[kv.first] =
                        std::min(cheapest_facts[kv.first], kv.second);
                }
            }
        }
        return EvalType<T>{cheapest_facts, cheapest_cost};
    };

    // Returns the cheapest cost of each fact
    auto eval_result =
        evmdd.template evaluate_partial<EvalType<T>>(state, eval);
    vector<pair<Fact, T>> result;
    for (auto const &kv : eval_result.first) {
        result.push_back(std::make_pair(kv.first, kv.second));
    }
    return result;
}

template vector<pair<Fact, int>> Relaxation::evaluate<int>(
    ProductEvmdd<Facts, int, Union, std::plus<int>> const &,
    PartialState const &);
// template std::vector<std::pair<Facts, double>> evaluate<double>(
//    ProductEvmdd<Facts, double, Union, std::plus<double>>);
// template std::vector<std::pair<Facts, float>> evaluate<float>(
//    ProductEvmdd<Facts, float, Union, std::plus<float>>);
