#include "evmdd.h"

using std::map;
using std::vector;
using std::pair;

map<Fact, int> operator+(
    map<Fact, int> &m,
    ProductMonoid<Facts, int, Union, std::plus<int>> const &monoid) {
    for (auto const &kv : m) {
        m[kv.first] += monoid.get_value().second;
    }
    for (Fact const &fact : monoid.get_value().first) {
        assert(m.find(fact) == m.end());
        m[fact] = monoid.get_value().second;
    }
    return m;
}

// Definitely not the fastest implementation
template <typename Cost>
vector<pair<Fact, Cost>> Relaxation::evaluate(
    ProductEvmdd<Facts, Cost, Union, std::plus<Cost>> const &evmdd,
    PartialState const &state) {
    // Only keep the cheapest cost for each fact
    auto eval = [](vector<map<Fact, Cost>> const &child_results) {
        map<Fact, Cost> result;
        for (map<Fact, Cost> const &child_result : child_results) {
            for (auto const &kv : child_result) {
                if (result[kv.first] < kv.second) {
                    result[kv.first] = kv.second;
                }
            }
        }
        return result;
    };

    // Returns the cheapest cost of each fact
    auto eval_result =
        evmdd.template evaluate_partial<map<Fact, Cost>>(state, eval);
    vector<pair<Fact, Cost>> result;
    for (auto const &kv : eval_result) {
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
