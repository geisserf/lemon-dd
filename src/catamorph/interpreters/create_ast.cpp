// Copyright 16.04.2018, University of Freiburg,
// Author: David Speck <speckd@informatik.uni-freiburg.de>.

#include "create_ast.h"
#include "../../parser.h"
#include <iomanip>
#include <iostream>
#include <stack>

ASTNode::ASTNode() : value("{}"), topo_order(-1), variable(false) {}

ASTNode::ASTNode(double value)
    : value(std::to_string(value)), topo_order(0), variable(false) {}

ASTNode::ASTNode(std::string value)
    : value(value), topo_order(0), variable(true) {}

ASTNode::ASTNode(std::string value, const std::vector<ASTNode>& children)
    : value(value), variable(false) {
    // Children are sorted according to their topo order
    this->children = children;
    std::sort(this->children.begin(), this->children.end(),
              [](const ASTNode& x, const ASTNode& y) {
                  return x.get_topo_order() < y.get_topo_order();
              });
    // Last child has highest topo value
    if (this->children.empty()) {
        topo_order = 0;
    } else {
        topo_order = this->children[children.size() - 1].get_topo_order() + 1;
    }
}

CreateAST::CreateAST() : root_node(ASTNode()) {}

ASTNode CreateAST::create_ast(std::string& term, std::vector<std::string>& variables) {
    Expression expression;
    try {
        std::string full_term = "(+ ";
        for (auto& var : variables) {
                full_term += var + " ";
        }
        full_term += term + ")";
        Parser prefix_parser;
        expression = prefix_parser.parse(full_term);
    } catch (std::exception const& infix_exception) {
        try {
            std::string full_term = "(0";
            for (auto& var : variables) {
                full_term += " + " + var;
            }
            full_term += " + " + term + ")";
            InfixParser infix_parser;
            expression = infix_parser.parse(full_term);
        } catch (std::exception const& prefix_exception) {
            std::cout << "Parser error: " << std::endl;
            std::cout << "Infix parser error: " << infix_exception.what()
                      << std::endl;
            std::cout << "Prefix parser error: " << prefix_exception.what()
                      << std::endl;
            throw std::invalid_argument("Invalid argument for parser.");
        }
    }
    return create_ast(expression);
}

std::vector<std::string> CreateAST::get_fan_in_ordering(const ASTNode& node,
                                                        int num_vars) {
    std::vector<std::string> var_order;

    std::stack<ASTNode> Q;
    std::vector<ASTNode> children;

    Q.push(node);

    while (!Q.empty()) {
        ASTNode cur = Q.top();
        Q.pop();

        if (static_cast<int>(var_order.size()) == num_vars)
            return var_order;

        // Is variable root node and not already contained in var_order
        if (cur.is_variable() &&
            std::find(var_order.begin(), var_order.end(), cur.get_value()) ==
                var_order.end()) {
            var_order.push_back(cur.get_value());
        }

        children = cur.get_children();

        // Children are already sorted by their topo order => push on stack
        for (size_t i = 0; i < children.size(); ++i) {
            Q.push(children[i]);
        }
    }
    std::reverse(var_order.begin(), var_order.end());
    return var_order;
}

void CreateAST::dump(const ASTNode& node, std::string indent) {
    std::cout << "[" << node.get_value() << " @ " << node.get_topo_order()
              << "] : " << std::endl;
    indent += " ";
    for (auto& c : node.get_children()) {
        std::cout << indent;
        dump(c, indent);
    }
}

void CreateAST::dump() {
    dump(root_node);
}
