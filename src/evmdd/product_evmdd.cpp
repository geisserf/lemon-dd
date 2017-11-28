#include "product_evmdd.h"

using std::map;
using std::vector;
using std::pair;

template <typename T>
EvaluationType<T> &EvaluationType<T>::operator+=(Weight<T> const &weight) {
    T edge_cost = weight.get_value().second;
    // Add edge cost to facts
    for (auto &kv : cheapest_facts) {
        kv.second += edge_cost;
    }
    // Add edge cost to current minimal cost
    cheapest_cost += edge_cost;
    // Add new facts to current facts with current cheapest cost
    for (Fact const &fact : weight.get_value().first) {
        assert(cheapest_facts.find(fact) == cheapest_facts.end());
        cheapest_facts[fact] = cheapest_cost;
    }
    return *this;
}

template <typename T>
vector<pair<Fact, T>> Relaxation::evaluate(
    ProductEvmdd<Facts, T, Union, std::plus<T>> const &evmdd,
    PartialState const &state) {
    // Store only cheapest cost for each fact and cheapest overall cost
    auto eval = [](vector<EvaluationType<T>> const &child_results) {
        EvaluationType<T> result;
        // Initialize result with max cost, such that we always update costs.
        result.cheapest_cost = std::numeric_limits<T>::max();
        for (auto const &child_result : child_results) {
            result.cheapest_cost =
                std::min(result.cheapest_cost, child_result.cheapest_cost);
            // Store facts with their cheapest cost
            map<Fact, T> const &child_facts = child_result.cheapest_facts;
            for (auto const &kv : child_facts) {
                if (result.cheapest_facts.find(kv.first) ==
                    result.cheapest_facts.end()) {
                    result.cheapest_facts[kv.first] = kv.second;
                } else {
                    result.cheapest_facts[kv.first] =
                        std::min(result.cheapest_facts[kv.first], kv.second);
                }
            }
        }
        return result;
    };

    // Returns the cheapest cost of each fact
    auto eval_result =
        evmdd.template evaluate_partial<EvaluationType<T>>(state, eval);
    vector<pair<Fact, T>> result;
    for (auto const &kv : eval_result.cheapest_facts) {
        result.push_back(std::make_pair(kv.first, kv.second));
    }
    return result;
}

template vector<pair<Fact, int>> Relaxation::evaluate<int>(
    ProductEvmdd<Facts, int, Union, std::plus<int>> const &,
    PartialState const &);
template vector<pair<Fact, double>> Relaxation::evaluate<double>(
    ProductEvmdd<Facts, double, Union, std::plus<double>> const &,
    PartialState const &);
template vector<pair<Fact, float>> Relaxation::evaluate<float>(
    ProductEvmdd<Facts, float, Union, std::plus<float>> const &,
    PartialState const &);
