#ifndef PRINTER_H
#define PRINTER_H

#include "node.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

template <typename M, typename F>
class Evmdd;

template <typename M, typename F>
class DotPrinter {
public:
    DotPrinter() = default;
    // prints the .dot representation of an evmdd
    void to_dot(std::ostream &out, Evmdd<M, F> const &evmdd,
                std::string arithmetic,
                std::vector<std::string> const &conditional) {
        node_count = 0;
        edge_count = 0;
        write_beginning(out, evmdd.get_input(), evmdd.get_source_node());
        process_nodes(out, evmdd.get_source_node());
        write_alignment(out);
        write_header(out, arithmetic, conditional);
        write_end(out);
        reset_internals();
    }

private:
    // Prints start of dot file and the first edge connecting to the entry node
    void write_beginning(std::ostream &out, Monoid<M, F> const &input_value,
                         Node_ptr<Monoid<M, F>> entry_node) const {
        out << "digraph G {" << std::endl;
        out << "dummy [style=invis];" << std::endl;
        out << "dummy -> \"dummy_weighted\" [arrowhead=none];";
        // Generating weighted node
        out << "\"dummy_weighted\" [shape=box, label=\""
            << input_value.to_string() << "\"];";
        // Weighted node generated
        out << "\"dummy_weighted\" -> \"" << entry_node->get_id()
            << "\" [dir=forward];" << std::endl;
    }
    // Returns conditional effects as string
    std::string get_cond_effects(
        std::vector<std::string> const &conditional) const {
        if (conditional.size() == 0) {
            return "<tr><td>none</td></tr>";
        }
        std::stringstream ss;
        for (auto effect : conditional) {
            ss << "<tr><td align=\"center\">" << effect << "</td></tr>";
        }
        std::string effects = ss.str();
        // Replace > in string to prevent syntax error in html
        for (std::string::size_type pos = 0;
             (pos = effects.find("->")) != std::string::npos; pos += 4) {
            effects.replace(pos, 2, "&rArr;");
        }
        return effects;
    }

    // Prints successor nodes and edges starting from entry_node
    void process_nodes(std::ostream &out, Node_ptr<Monoid<M, F>> entry_node) {
        if (printed_nodes.find(entry_node) != printed_nodes.end()) {
            return;
        }
        print_node(out, entry_node);
        printed_nodes.insert(entry_node);
        same_level_nodes[entry_node->get_level()].push_back(entry_node);
        int i = 0;
        for (Edge<Monoid<M, F>> const &edge : entry_node->get_children()) {
            print_edge(out, entry_node, edge.first, edge.second, i);
            process_nodes(out, edge.second);
            ++i;
        }
    }

    // Prints the node
    void print_node(std::ostream &out, Node_ptr<Monoid<M, F>> node) {
        out << "\"" << node->get_id() << "\"";
        out << "[style=filled, fillcolor=lightgrey, label=\"";
        out << node->get_variable() << "\"];" << std::endl;
        node_count++;
    }

    // Prints an edge between two nodes with its weight.
    // Note: We omit domain values, because graphviz has really bad alignment
    // for multiple labels on one edge.
    void print_edge(std::ostream &out, Node_ptr<Monoid<M, F>> parent,
                    Monoid<M, F> weight, Node_ptr<Monoid<M, F>> child,
                    int domain) {
        out << "\"" << parent->get_id() << "\" -> \"" << parent->get_id()
            << domain << child->get_id() << "\""
            << " [arrowhead=none, label=\"" << domain << "\"];";
        // Generating weighted node
        out << "\"" << parent->get_id() << domain << child->get_id() << "\""
            << "[shape=box, label=\"" << weight.to_string() << "\"];";
        // Weighted node generated
        out << "\"" << parent->get_id() << domain << child->get_id()
            << "\" -> \"" << child->get_id() << "\" [dir=forward];"
            << std::endl;
        edge_count++;
    }

    // Aligns nodes on the same level
    void write_alignment(std::ostream &out) const {
        for (auto const &kv_pair : same_level_nodes) {
            out << "{rank=same;";
            for (Node_ptr<Monoid<M, F>> node : kv_pair.second) {
                out << "\"" << node->get_id() << "\";";
            }
            out << "}" << std::endl;
        }
    }
    // Prints an informative header about EVMDD to the top
    void write_header(std::ostream &out, std::string arithmetic,
                      std::vector<std::string> const &conditional) const {
        int con_count = conditional.size() + 2;
        std::string copy_arithmetic = arithmetic;
        out << "labelloc=\"t\";" << std::endl;
        out << "label=<<table cellborder=\"0\">";
        // Expressions
        out << "<tr><td border=\"0\" align=\"center\" colspan=\"2\">";
        out << "EVMDD with</td></tr>";
        out << "<tr><td border=\"1\" align=\"center\">";
        out << "Arithmetic Expression:</td>";
        out << "<td border=\"1\" align=\"center\">";
        out << "Conditional Effects:</td></tr>";
        out << "<tr><td rowspan=\"" << con_count << "\" align=\"center\">";
        // Adjust arithmetic expression length if too long
        if (arithmetic.size() > 15) {
            copy_arithmetic = arithmetic.substr(0, 12);
            copy_arithmetic += "...";
        }
        out << copy_arithmetic << "</td></tr>";
        out << get_cond_effects(conditional);
        // Legend
        out << "<tr><td border=\"1\" colspan=\"2\">Legend:</td></tr>";
        out << "<tr><td colspan=\"2\">#Nodes: " << node_count << "</td></tr>";
        out << "<tr><td colspan=\"2\">#Edges: " << edge_count << "</td></tr>";
        out << "</table>>;" << std::endl;
    }

    void write_end(std::ostream &out) const {
        out << "}" << std::endl;
    }

    void reset_internals() {
        same_level_nodes.clear();
        printed_nodes.clear();
    }

    // Store which nodes lie on the same level for prettier visualization
    std::map<int, std::vector<Node_ptr<Monoid<M, F>>>> same_level_nodes;
    // Nodes which were already printed
    std::unordered_set<Node_ptr<Monoid<M, F>>> printed_nodes;
    int node_count;
    int edge_count;
};

#endif /* PRINTER_H */
