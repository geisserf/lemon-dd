#ifndef NUMERIC_CATAMORPH_EVMDD_H
#define NUMERIC_CATAMORPH_EVMDD_H

#include "evmdd_cache.h"
#include "monoid.h"
#include "node.h"
#include "operations/logic_and.h"
#include "operations/logic_not.h"
#include "operations/logic_or.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <map>
#include <numeric>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

using Ordering = std::vector<std::string>;
using ConcreteState = std::vector<unsigned int>;

template <typename M, typename F = std::plus<M>>
class EvmddFactory;

template <typename M, typename F = std::plus<M>>
class Evmdd {
private:
    Monoid<M, F> input;
    Node_ptr<Monoid<M, F>> source_node;

    explicit Evmdd(Monoid<M, F> const &input, Node_ptr<Monoid<M, F>> source)
        : input(input), source_node(source) {}

    explicit Evmdd(M const &input, Node_ptr<Monoid<M, F>> source)
        : input(input), source_node(source) {}

    template <typename N, typename G>
    friend class EvmddFactory;

public:
    Evmdd() = default;

    // Prints nodes in a BFS-like manner beginning from the source node.
    void print(std::ostream &out) const {
        out << "input: " << input.to_string() << std::endl;
        std::queue<Node_ptr<Monoid<M, F>>> nodes;
        nodes.push(source_node);
        std::unordered_set<Node_ptr<Monoid<M, F>>> processed;
        while (!nodes.empty()) {
            auto current = nodes.front();
            nodes.pop();
            // Do not print terminal nodes
            if (current->is_terminal()) {
                continue;
            }
            out << current->to_string() << std::endl;
            for (auto const &edge : current->get_children()) {
                if (processed.find(edge.second) == processed.end()) {
                    nodes.push(edge.second);
                    processed.insert(edge.second);
                }
            }
        }
    }

    // Evaluates the evmdd with the given state.
    // If the evmdd represents function E:S->M, this computes E(s).
    // Assumes state is ordered according to evmdd ordering.
    M evaluate(ConcreteState const &state) const {
        Monoid<M, F> res = input;
        auto node = source_node;
        while (!node->is_terminal()) {
            assert(node->get_level() <= state.size());
            // TODO implement correct ordering
            size_t domain_value = state[node->get_level() - 1];
            assert(domain_value < node->get_children().size());
            res += node->get_children()[domain_value].first;
            node = node->get_children()[domain_value].second;
        }
        return res.get_value();
    }

    // Evaluates the evmdd given a partial state, according to some evaluation
    // strategy. When evaluating a node, the evaluation strategy decides for
    // a node how the results of its children are 'merged' together to return
    // type Res.
    //
    // An example for evmdds over numbers is to use min/max as evaluation
    // function to return the minimal/maximal cost for a partial state.

    template <typename Res, typename EvaluationFunction>
    Res evaluate_partial(PartialState const &state,
                         EvaluationFunction func) const {
        Res source_result = source_node->template evaluate<Res>(state, func);
        // TODO: Instead of clearing the whole cache, we could think about
        // storing the source node result, since this may more likely be used
        // for future requests.
        EvalCache<EvaluationFunction, Res, Monoid<M, F>>::clear();
        return source_result + input;
    }

    // Computes min_{s \in S} E(s)
    M get_min() const {
        auto complete_state = std::map<std::string, std::vector<int>>();
        auto eval = [](std::vector<Monoid<M, F>> const &vec) {
            return *std::min_element(vec.begin(), vec.end());
        };
        auto min = evaluate_partial<Monoid<M, F>>(complete_state, eval);
        return min.get_value();
    }

    // Computes max_{s \in S} E(s)
    M get_max() const {
        auto complete_state = std::map<std::string, std::vector<int>>();
        auto eval = [](std::vector<Monoid<M, F>> const &vec) {
            return *std::max_element(vec.begin(), vec.end());
        };
        auto max = evaluate_partial<Monoid<M, F>>(complete_state, eval);
        return max.get_value();
    }

    Node_ptr<Monoid<M, F>> const &get_source_node() const {
        assert(source_node);
        return source_node;
    }

    Monoid<M, F> get_input() const {
        return input;
    }

    // Returns the number of nodes
    int size() const {
        std::unordered_set<Node_ptr<Monoid<M, F>>> succ;
        source_node->unique_successor_nodes(succ);
        return succ.size() + 1; // counts entry node
    }

    int depth() const {
        return source_node->get_depth();
    }

    // Check if the EVMDD does exist (e.g. default constructor EVMDD is invalid)
    bool exists() const {
        return static_cast<bool>(source_node);
    }

    friend bool operator<(const Evmdd<M, F> &l, const Evmdd<M, F> &r) {
        if (l.source_node == r.source_node) {
            return l.input < r.input;
        }
        return l.source_node < r.source_node;
    }

    friend bool operator==(const Evmdd<M, F> &l, const Evmdd<M, F> &r) {
        return ((l.input == r.input) && (l.source_node == r.source_node));
    }
};

template <typename M, typename F>
class AbstractFactory;

template <typename M, typename F>
class EvmddFactory {
public:
    // Interaction with factories is only accessible via AbstractFactory.
    EvmddFactory(EvmddFactory const &other) = delete;

    // Number of stored nodes
    size_t size() const {
        return node_factory.size();
    }

    // Creates an evmdd for a constant term
    Evmdd<M, F> make_const_evmdd(M const &weight) {
        return Evmdd<M, F>(weight, node_factory.get_terminal_node());
    }

    // Creates an evmdd for a function which maps variable domain values to M.
    // This results in one node representing the variable, connected to the
    // terminal node, each edge annotated with the given domain value.
    Evmdd<M, F> make_var_evmdd(std::string const &var,
                               std::vector<M> const &domain) {
        std::vector<Edge<Monoid<M, F>>> children;
        for (size_t i = 0; i < domain.size(); ++i) {
            children.emplace_back(Monoid<M, F>(domain[i]),
                                  node_factory.get_terminal_node());
        }
        // If variable is not indicated in ordering, append it at the end
        auto var_it = std::find(ordering.begin(), ordering.end(), var);
        if (var_it == ordering.end()) {
            ordering.push_back(var);
            var_it = ordering.end() - 1;
        }
        // Increase by 1 since index 0 is reserved for terminal node
        auto var_pos = std::distance(ordering.begin(), var_it) + 1;
        Node_ptr<Monoid<M, F>> node =
            node_factory.make_node(var_pos, var, children);
        return Evmdd<M, F>(Monoid<M, F>::neutral_element(), node);
    }

    // Given two evmdds 'left', 'right' over monoids <L,G> and <R,H> and given
    // operator op: LxR->M, apply returns an evmdd over monoid <M,F> which
    // represents function 'left op right'.
    template <typename L, typename G, typename R, typename H, typename OP>
    Evmdd<M, F> apply(Evmdd<L, G> const &left, Evmdd<R, H> const &right,
                      OP oper) {
        if (terminal_case(left, right, oper)) {
            return make_terminal_evmdd(left, right, oper);
        }
        // Check if computation is cached
        auto it = EvmddCache<Evmdd<M, F>, Evmdd<L, G>, Evmdd<R, H>, OP>::find(
            left, right);
        if (it !=
            EvmddCache<Evmdd<M, F>, Evmdd<L, G>, Evmdd<R, H>, OP>::end()) {
            return it->second;
        }
        std::vector<Evmdd<L, G>> left_sub_evmdds = sub_evmdds(left, right);
        std::vector<Evmdd<R, H>> right_sub_evmdds = sub_evmdds(right, left);
        assert(left_sub_evmdds.size() > 0);
        assert(left_sub_evmdds.size() == right_sub_evmdds.size());

        // TODO variable should be retrieved by the ordering and not stored in
        // nodes
        int root_level;
        std::string var;
        if (left.get_source_node()->get_level() <=
            right.get_source_node()->get_level()) {
            root_level = right.source_node->get_level();
            var = right.source_node->get_variable();
        } else {
            root_level = left.get_source_node()->get_level();
            var = left.get_source_node()->get_variable();
        }

        std::vector<Evmdd<M, F>> new_children;
        new_children.reserve(left_sub_evmdds.size());
        for (size_t i = 0; i < left_sub_evmdds.size(); ++i) {
            new_children.push_back(
                apply(left_sub_evmdds[i], right_sub_evmdds[i], oper));
        }
        Evmdd<M, F> result = create_evmdd(root_level, var, new_children);
        // Cache result
        EvmddCache<Evmdd<M, F>, Evmdd<L, G>, Evmdd<R, H>, OP>::add(left, right,
                                                                   result);
        return result;
    }

    // Product of two evmdds. Only defined for ProductMonoids.
    template <typename L, typename G, typename R, typename H>
    Evmdd<M, F> product(Evmdd<L, G> const &left, Evmdd<R, H> const &right) {
        // Compile time error message if a user reverses the arguments.
        static_assert(!std::is_same<M, std::pair<R, L>>::value,
                      "User Error: product arguments are reversed.");
        // Compile time error message if a user calls the product for a
        // factory which is not of type M=pair<L,R>
        static_assert(!std::is_same<M, std::pair<R, L>>::value ||
                          std::is_same<M, std::pair<L, R>>::value,
                      "User Error: M is not a pair of L and R. Product of two "
                      "evmdds has to be called by a ProductFactory");
        return apply(left, right, std::make_pair<L, R>);
    }

private:
    // Returns true if the operation can immmediately be computed between the
    // evmdds.
    template <typename L, typename G, typename R, typename H, typename OP>
    bool terminal_case(Evmdd<L, G> const &left, Evmdd<R, H> const &right,
                       OP /*oper*/) {
        // TODO use oper to determine lazy termination
        return (left.get_source_node()->is_terminal() &&
                right.get_source_node()->is_terminal());
    }

    // computation of 'left oper right' if it is a terminal operation
    template <typename L, typename G, typename R, typename H, typename OP>
    Evmdd<M, F> make_terminal_evmdd(Evmdd<L, G> const &left,
                                    Evmdd<R, H> const &right, OP oper) {
        // oper:LxR->M implies that we can apply it on the carrier types instead
        // of the monoid itself.
        M input =
            oper(left.get_input().get_value(), right.get_input().get_value());
        return make_const_evmdd(input);
    }

    // If level of f is smaller than level of g, copy f |g.children| times.
    // Otherwise create an evmdd for each child of f with (weight+input value)
    // as new input value and the child as root node.
    template <typename L, typename G, typename R, typename H>
    std::vector<Evmdd<L, G>> sub_evmdds(Evmdd<L, G> const &f,
                                        Evmdd<R, H> const &g) {
        std::vector<Evmdd<L, G>> result;
        result.reserve(g.get_source_node()->get_children().size());
        if (f.get_source_node()->get_level() >=
            g.get_source_node()->get_level()) {
            for (auto const &edge : f.get_source_node()->get_children()) {
                Monoid<L, G> input = f.get_input() + edge.first;
                Evmdd<L, G> evmdd(input, edge.second);
                result.push_back(evmdd);
            }
        } else {
            for (size_t i = 0; i < g.get_source_node()->get_children().size();
                 ++i) {
                result.push_back(f);
            }
        }
        return result;
    }

    // Returns an evmdd with root node at level, with input value as the
    // greatest lower bound of the input value of all children.
    // Weights to each child is its original input value minus the greatest
    // lower bound.
    Evmdd<M, F> create_evmdd(int level, std::string var,
                             std::vector<Evmdd<M, F>> const &children) {
        // Reduction requirement: if all children are equal, we can immediately
        // return the child as new EVMDD.
        if (std::all_of(children.begin() + 1, children.end(),
                        [&](Evmdd<M, F> const &child) {
                            return child == children[0];
                        })) {
            return children[0];
        }

        // Compute greatest lower bound of all children
        auto it = std::next(children.begin());
        M const &first_value = children[0].get_input().get_value();
        M min_weight_value = std::accumulate(
            it, children.end(), first_value, [](M l, Evmdd<M, F> const &r) {
                return Monoid<M, F>::greatest_lower_bound(
                    l, r.get_input().get_value());
            });
        Monoid<M, F> min_weight{min_weight_value};
        std::vector<Edge<Monoid<M, F>>> edges;
        edges.reserve(children.size());
        for (Evmdd<M, F> const &child : children) {
            edges.emplace_back(child.get_input() - min_weight,
                               child.get_source_node());
        }
        Node_ptr<Monoid<M, F>> root_node =
            node_factory.make_node(level, var, edges);
        return Evmdd<M, F>(min_weight, root_node);
    }

    EvmddFactory(Ordering const &o) : ordering(o) {}

    // variable ordering
    Ordering ordering;
    NodeFactory<Monoid<M, F>> node_factory;

    friend AbstractFactory<M, F>;
};

#endif // NUMERIC_CATAMORPH_EVMDD_H
