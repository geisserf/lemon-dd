#ifndef PRINTER_H
#define PRINTER_H

#include "node.h"

#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

template <typename M, typename F>
class Evmdd;

template <typename M, typename F>
class DotPrinter {
public:
    DotPrinter() = default;
    // prints the .dot representation of an evmdd
    void to_dot(std::ostream &out, Evmdd<M, F> const &evmdd) {
        node_count = 0;
        edge_count = 0;
        write_header(out, evmdd.get_input(), evmdd.get_source_node());
        process_nodes(out, evmdd.get_source_node());
        write_alignment(out);
        print_legend(out);
        // print_evmdd_formula(out, evmdd);
        write_end(out);
        reset_internals();
    }

private:
    void print_legend(std::ostream &out) {
        out << "node [shape=plaintext]";
        out << "subgraph cluster_01 {";
        out << "label = \"Legend\";";
        out << "key [label=<<table border=\"0\" cellpadding=\"2\" "
               "cellspacing=\"0\" cellborder=\"0\">";
        out << "  <tr><td align=\"right\" port=\"i1\">#Nodes:</td><td>"
            << node_count << "</td></tr>";
        out << "  <tr><td align=\"right\" port=\"i2\">#Edges:</td><td>"
            << edge_count << "</td></tr>";
        out << "  </table>>]";
        out << "}";
    }

    // TODO: For more informative output, implement the proper function
    //       (?) How to get the evmdd formula (from evmdd object?) ?
    //       (@etavas)
    //
    // Prints evmdd formula
    /*
    void print_evmdd_formula(std::ostream &out, Evmdd<M, F> const &evmdd) {
        out << "subgraph cluster_02 {";
        out << "label = \"EVMDD formula\";";
        out << "key [label=<<table border=\"0\" cellpadding=\"2\" "
               "cellspacing=\"0\" cellborder=\"0\">";
        out << "  <tr><td align=\"right\" port=\"i1\">Arithmetic "
               "formula:</td><td>"
            << evmdd.get_input().to_string() << "</td></tr>";
        out << "  <tr><td align=\"right\" port=\"i2\">Conditional "
               "effects:</td><td>"
            << evmdd.get_input().to_string() << "</td></tr>";
        out << "  </table>>]";
        out << "}";
    }*/

    // Prints start of dot file and the first edge connecting to the entry node
    void write_header(std::ostream &out, Monoid<M, F> const &input_value,
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
