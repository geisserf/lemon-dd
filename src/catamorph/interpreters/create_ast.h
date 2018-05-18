// Copyright 16.04.2018, University of Freiburg,
// Author: David Speck <speckd@informatik.uni-freiburg.de>.

#ifndef CREATE_AST_H_
#define CREATE_AST_H_

#include "../catamorph.h"
#include "../expression.h"
#include <experimental/type_traits>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class ASTNode {
    std::string value;             // Var, Const or operator string
    int topo_order;                // Topological order
    std::vector<ASTNode> children; // Sorted by the topological order (incr)
    bool variable;                 // Root variable node?

public:
    ASTNode();

    ASTNode(std::string value); // Note: pass by value for rvalues support

    ASTNode(double value);

    ASTNode(std::string value, const std::vector<ASTNode>& children);

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

class CreateAST {
private:
    ASTNode root_node; // Root node of the AST

    auto create_ast_alg() {
        return [this](expression_r<ASTNode> const& e) -> ASTNode {

            if (const NBR* cst = Factories::get_as_cst(e)) {
                return ASTNode(*cst);
            }
            if (auto* var = Factories::get_as_var(e)) {
                return ASTNode(*var);
            }
            if (auto* o = Factories::get_as_add(e)) {
                return ASTNode("+", o->rands());
            }
            if (auto* o = Factories::get_as_sub(e)) {
                return ASTNode("-", o->rands());
            }
            if (auto* o = Factories::get_as_mul(e)) {
                return ASTNode("*", o->rands());
            }
            if (auto* o = Factories::get_as_div(e)) {
                return ASTNode("/", o->rands());
            }
            //if (auto* o = Factories::get_as_pow(e)) {
            //    return ASTNode("^", o->rands());
            //}
            //if (auto* o = Factories::get_as_greater(e)) {
            //    return ASTNode(">", o->rands());
            //}
            //if (auto* o = Factories::get_as_greater_equals(e)) {
            //    return ASTNode(">=", o->rands());
            //}
            if (auto* o = Factories::get_as_and(e)) {
                return ASTNode("&", o->rands());
            }
            if (auto* o = Factories::get_as_equals(e)) {
                return ASTNode("==", o->rands());
            }
            if (auto* o = Factories::get_as_or(e)) {
                return ASTNode("|", o->rands());
            }
            if (auto* o = Factories::get_as_not(e)) {
                assert(o->rands().size() == 1);
                return ASTNode("!", o->rands());
            }
            throw std::logic_error("Unknown Operator in AST");
        };
    }

    ASTNode create_ast(Expression const& expr) {
        root_node = Catamorph::cata<ASTNode>(
            [this](expression_r<ASTNode> const& expr_r) -> ASTNode {
                return create_ast_alg()(expr_r);
            },
            expr);
        return root_node;
    }

public:
    CreateAST();

    ASTNode create_ast(std::string& term, std::vector<std::string>& variables);

    void dump(const ASTNode& node, std::string indent = "");
    void dump();

    // Returns variable importance in increasing order
    // => [a, c, b] means a < c < b (regarding importance)
    // => level(b) > level(c) > level(a) if terminal t has level(t) = 0
    std::vector<std::string> get_fan_in_ordering(const ASTNode& node,
                                                 int numvars = -1);
};
#endif // CREATE_AST_H_