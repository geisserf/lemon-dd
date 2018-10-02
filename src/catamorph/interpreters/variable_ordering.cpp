#include "variable_ordering.h"
#include "../../parser.h"
#include <iomanip>
#include <iostream>
#include <stack>

ASTNode::ASTNode() : value("{}"), topo_order(-1), variable(false) {}

ASTNode::ASTNode(double value)
    : value(std::to_string(value)), topo_order(0), variable(false) {}

ASTNode::ASTNode(const std::string &value)
    : value(value), topo_order(0), variable(true) {}

ASTNode::ASTNode(const std::string &value, const std::vector<ASTNode> &children)
    : value(value), variable(false) {
    // Children are sorted according to their topo order
    this->children = children;
    std::sort(this->children.begin(), this->children.end(),
              [](const ASTNode &x, const ASTNode &y) {
                  return x.get_topo_order() < y.get_topo_order();
              });
    // Last child has highest topo value
    if (this->children.empty()) {
        topo_order = 0;
    } else {
        topo_order = this->children[children.size() - 1].get_topo_order() + 1;
    }
}

ASTNode VariableOrdering::create_ast(
    const std::string &term, const std::vector<std::string> &variables) {
    Expression expression;
    try {
        std::string full_term = "(+ ";
        for (auto &var : variables) {
            full_term += var + " ";
        }
        full_term += term + ")";
        Parser prefix_parser;
        expression = prefix_parser.parse(full_term);
    } catch (std::exception const &infix_exception) {
        try {
            std::string full_term = "(0";
            for (auto &var : variables) {
                full_term += " + " + var;
            }
            full_term += " + " + term + ")";
            InfixParser infix_parser;
            expression = infix_parser.parse(full_term);
        } catch (std::exception const &prefix_exception) {
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

std::vector<std::string> VariableOrdering::get_fan_in_ordering(
    const std::string &term, const std::vector<std::string> &variables) {
    auto node = create_ast(term, variables);
    std::vector<std::string> var_order;

    std::stack<ASTNode> Q;
    std::vector<ASTNode> children;

    Q.push(node);

    while (!Q.empty()) {
        ASTNode cur = Q.top();
        Q.pop();

        if (var_order.size() == variables.size())
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
