#include "variable_ordering.h"
#include "../../parser.h"
#include "dependencies.h"
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

std::vector<std::string> VariableOrdering::get_fan_in_ordering(
    const Expression &expr) {
    size_t num_supp_vars = Dependency::dependencies(expr).size();
    auto node = create_ast(expr);
    std::vector<std::string> var_order;

    std::stack<ASTNode> Q;
    std::vector<ASTNode> children;

    Q.push(node);

    while (!Q.empty()) {
        ASTNode cur = Q.top();
        Q.pop();

        if (var_order.size() == num_supp_vars)
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
