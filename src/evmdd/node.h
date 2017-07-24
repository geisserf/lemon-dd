#ifndef NODE_H
#define NODE_H

#include "evmdd_expression.h"
#include "node_storage.h"
#include <map>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>

template <typename T>
class Label {
public:
    Label(T expr) : expression(expr) {}
    T expression;
};

template <typename T>
class Node;

template <typename T>
using Edge = std::pair<Label<T>, int>;
using State = std::map<std::string, std::vector<int>>;

template <typename T>
class NodeFactory;

template <typename T>
class Node {
private:
    //    Node() {}
    Node(int id, int level, std::string const &var,
         std::vector<Edge<T>> const &children, NodeStorage<T> *storage);

    int id;
    int level;
    std::string variable;
    std::vector<Edge<T>> children;
    NodeStorage<T> *storage;

    friend NodeFactory<T>;
    friend NodeStorage<T>;

public:
    void print(std::ostream &out, std::string indent = "") const;

    int get_id() const {
        return id;
    }
    std::vector<Edge<T>> get_children() const {
        return children;
    }

    std::string get_variable() const {
        return variable;
    }

    int get_level() const {
        return level;
    }
    // template <typename EvaluationFunction>
    // std::vector<T> evaluate(State const &state, EvaluationFunction func);
    template <typename EvaluationFunction>
    std::vector<T> evaluate(State const &state, EvaluationFunction func) {
        (void)func;
        std::vector<T> result;
        auto state_values_iter = state.find(variable);
        if (state_values_iter != state.end()) {
            std::vector<int> state_values = state_values_iter->second;
            for (size_t i = 0; i < state_values.size(); ++i) {
                if (children[state_values[i]].second == 0) {
                    result = func(children[state_values[i]].first.expression,
                                  result);
                } else {
                    Node<T> c_node = storage->get_node(children[i].second);
                    std::vector<T> c_result = c_node.evaluate(state, func);
                    // add weight to child evaluations:
                    for (T c_e : c_result) {
                        T current =
                            c_e + children[state_values[i]].first.expression;
                        result = func(current, result);
                    }
                }
            }
        } else {
            for (size_t i = 0; i < children.size(); ++i) {
                if (children[i].second == 0) {
                    result = func(children[i].first.expression, result);
                } else {
                    // get evaluation of child
                    Node<T> c_node = storage->get_node(children[i].second);
                    std::vector<T> c_result = c_node.evaluate(state, func);
                    // add weight to child evaluations:
                    for (T c_e : c_result) {
                        T current = c_e + children[i].first.expression;
                        result = func(current, result);
                    }
                }
            }
        }
        return result;
    }
};

template <typename T>
class NodeFactory {
public:
    NodeFactory();
    Node<T> get_terminal_node();
    Node<T> get_node(int id) {
        return storage.get_node(id);
    }
    Node<T> make_node(int level, std::string const &variable,
                      std::vector<Edge<T>> const &children);

private:
    NodeStorage<T> storage;
    int node_counter;
};

#endif /* NODE_H */
