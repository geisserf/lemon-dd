#ifndef VARIABLE_ORDERING_H_
#define VARIABLE_ORDERING_H_

#include "../catamorph.h"
#include "../expression.h"
#include <experimental/type_traits>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// AST Node which can be used to compute variable oderings

class ASTNode {
    std::string value;             // Var, Const or operator string
    int topo_order;                // Topological order
    std::vector<ASTNode> children; // Sorted by the topological order (incr)
    bool variable;                 // Root variable node?

public:
    ASTNode();

    ASTNode(const std::string &value);

    ASTNode(double value);

    ASTNode(const std::string &value, const std::vector<ASTNode> &children);

    std::string get_value() const {
        return value;
    }

    int get_topo_order() const {
        return topo_order;
    }

    std::vector<ASTNode> get_children() const {
        return children;
    }

    bool is_leaf() const {
        return children.empty();
    }

    bool is_variable() const {
        return variable;
    }
};

class VariableOrdering {
protected:
    auto create_ast_alg() {
        return [this](expression_r<ASTNode> const &e) -> ASTNode {
            if (const NBR *cst = Factories::get_as_cst(e)) {
                return ASTNode(*cst);
            }
            if (auto *var = Factories::get_as_var(e)) {
                return ASTNode(*var);
            }
            if (auto *o = Factories::get_as_add(e)) {
                return ASTNode("+", o->rands());
            }
            if (auto *o = Factories::get_as_sub(e)) {
                return ASTNode("-", o->rands());
            }
            if (auto *o = Factories::get_as_mul(e)) {
                return ASTNode("*", o->rands());
            }
            if (auto *o = Factories::get_as_div(e)) {
                return ASTNode("/", o->rands());
            }
            if (auto* o = Factories::get_as_abs(e)) {
                return ASTNode("abs", o->rands());
            }
            if (auto* o = Factories::get_as_greater_equals(e)) {
                return ASTNode(">=", o->rands());
            }
            if (auto* o = Factories::get_as_greater(e)) {
                return ASTNode(">", o->rands());
            }
            if (auto* o = Factories::get_as_less_equals(e)) {
                return ASTNode("<=", o->rands());
            }
            if (auto *o = Factories::get_as_less(e)) {
                return ASTNode("<", o->rands());
            }
            if (auto *o = Factories::get_as_equals(e)) {
                return ASTNode("==", o->rands());
            }
            if (auto *o = Factories::get_as_and(e)) {
                return ASTNode("&", o->rands());
            }
            if (auto *o = Factories::get_as_or(e)) {
                return ASTNode("|", o->rands());
            }
            if (auto *o = Factories::get_as_not(e)) {
                assert(o->rands().size() == 1);
                return ASTNode("!", o->rands());
            }
            throw std::logic_error("Unknown Operator in AST");
        };
    }

    ASTNode create_ast(Expression const &expr) {
        return Catamorph::cata<ASTNode>(
            [this](expression_r<ASTNode> const &expr_r) -> ASTNode {
                return create_ast_alg()(expr_r);
            },
            expr);
    }

    ASTNode create_ast(const std::string &term,
                       const std::vector<std::string> &variables);

public:
    // All variables of the current problem can be passed. At least all
    // variables of occuring in the term should be passed. Returns variable
    // importance in increasing order
    // => [a, c, b] means a < c < b (regarding importance)
    // => level(b) > level(c) > level(a) if terminal t has level(t) = 0
    std::vector<std::string> get_fan_in_ordering(
        const std::string &term, const std::vector<std::string> &variables);
};
#endif // VARIABLE_ORDERING_H_
