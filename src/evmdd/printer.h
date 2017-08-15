#ifndef PRINTER_H
#define PRINTER_H

#include "node.h"

#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

using Ordering = std::map<std::string, int>;

template <typename T>
class Evmdd;

template <typename T>
class DotPrinter {
public:
    DotPrinter(Ordering const &ordering) : ordering(ordering) {}

    // prints the .dot representation of an evmdd
    void to_dot(std::ostream &out, Evmdd<T> const &evmdd) {
        write_header(out, evmdd.get_input_value(), evmdd.get_entry_node());
        process_nodes(out, evmdd.get_entry_node());
        write_alignment(out);
        write_end(out);
        reset_internals();
    }

private:
    // Prints start of dot file and the first edge connecting to the entry node
    void write_header(std::ostream &out, T const &input_value,
                      Node_ptr<T> entry_node) const {
        out << "digraph G {" << std::endl;
        out << "dummy [style=invis];" << std::endl;
        out << "dummy -> \"" << entry_node->get_id() << "\"";
        out << "[arrowhead=none, label=\"" << input_value.toString() << "\"];"
            << std::endl;
    }

    // Prints successor nodes and edges starting from entry_node
    void process_nodes(std::ostream &out, Node_ptr<T> entry_node) {
        if (printed_nodes.find(entry_node) != printed_nodes.end()) {
            return;
        }
        print_dot(out, entry_node);
        printed_nodes.insert(entry_node);

        // TODO add level
        int level = entry_node->get_level();
        same_level_nodes[level].push_back(entry_node);

        for (Edge<T> const &edge : entry_node->get_children()) {
            print_edge(out, entry_node, edge.first, edge.second);
            process_nodes(out, edge.second);
        }
    }

    // Prints the node
    void print_dot(std::ostream &out, Node_ptr<T> node) const {
        out << "\"" << node->get_id() << "\"";
        out << "[style=filled, fillcolor=lightgrey, label=\"";
        out << node->get_variable() << "\"];" << std::endl;
    }

    // Prints an edge between two nodes with its weight.
    // Note: We omit domain values, because graphviz has really bad alignment
    // for multiple labels on one edge.
    void print_edge(std::ostream &out, Node_ptr<T> parent, T weight,
                    Node_ptr<T> child) const {
        out << "\"" << parent->get_id() << "\" -> \"" << child->get_id()
            << "\"";
        out << " [arrowhead=none,label=\"" << weight.toString() << "\"];"
            << std::endl;
    }

    // Aligns nodes on the same level
    void write_alignment(std::ostream &out) const {
        for (auto const &kv_pair : same_level_nodes) {
            out << "{rank=same;";
            for (Node_ptr<T> node : kv_pair.second) {
                out << "\"" << node->get_id() << "\";";
            }
            out << "}" << std::endl;
        }
    }

    void write_end(std::ostream &out) const {
        out << "}" << std::endl;
    }

    void reset_internals() {
        same_level_nodes.clear();
        printed_nodes.clear();
    }

    Ordering ordering;
    // Store which nodes lie on the same level for prettier visualization
    std::map<int, std::vector<Node_ptr<T>>> same_level_nodes;
    // Nodes which were already printed
    std::unordered_set<Node_ptr<T>> printed_nodes;
};

#endif /* PRINTER_H */
