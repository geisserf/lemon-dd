#ifndef PRINTER_H
#define PRINTER_H

#include "node.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

template <typename M, typename F>
class Evmdd;

using std::string;

template <typename M, typename F>
class MermaidPrinter {
public:
    MermaidPrinter() = default;
    // prints the .dot representation of an evmdd
    void to_mermaid(std::ostream &out, string const & /*filename*/,
                    Evmdd<M, F> const &evmdd, string const & /*arithmetic*/,
                    std::vector<string> const & /*conditional*/,
                    string::size_type /*max_width*/) {
        node_count = 0;
        edge_count = 0;
        write_beginning(out, evmdd.get_input(), evmdd.get_source_node());
        process_nodes(out, evmdd.get_source_node());
        reset_internals();
    }

private:
    string write_mermaid_node(Node_ptr<Monoid<M, F>> const &node) const {
        string result = std::to_string(node->get_id());
        result += "(" + node->get_variable();
        result += ")";
        return result;
    }

    // Prints start of dot file and the first edge connecting to the entry node
    void write_beginning(std::ostream &out, Monoid<M, F> const &input_value,
                         Node_ptr<Monoid<M, F>> entry_node) const {
        out << "graph TD" << std::endl;
        out << "dummy[ ] --> |" << input_value.to_string() << "|"
            << write_mermaid_node(entry_node) << std::endl;
    }

    // Prints successor nodes and edges starting from entry_node
    void process_nodes(std::ostream &out, Node_ptr<Monoid<M, F>> entry_node) {
        if (printed_nodes.find(entry_node) != printed_nodes.end()) {
            return;
        }
        printed_nodes.insert(entry_node);
        int i = 0;
        for (Edge<Monoid<M, F>> const &edge : entry_node->get_children()) {
            print_edge(out, entry_node, edge.first, edge.second, i);
            process_nodes(out, edge.second);
            ++i;
        }
    }

    // Prints an edge between two nodes with its weight.
    void print_edge(std::ostream &out, Node_ptr<Monoid<M, F>> parent,
                    Monoid<M, F> weight, Node_ptr<Monoid<M, F>> child,
                    int domain) {
        string weight_id = std::to_string(parent->get_id());
        weight_id += std::to_string(domain);
        weight_id += std::to_string(child->get_id());
        out << write_mermaid_node(parent) << " --> |" << domain << "|"
            << weight_id << "(" << weight.to_string() << ")" << std::endl;
        out << weight_id << " --> " << write_mermaid_node(child) << std::endl;
        edge_count++;
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

template <typename M, typename F>
class DotPrinter {
public:
    DotPrinter() = default;
    // prints the .dot representation of an evmdd
    void to_dot(std::ostream &out, std::string const &filename,
                Evmdd<M, F> const &evmdd, std::string const &arithmetic,
                std::vector<std::string> const &conditional,
                std::string::size_type max_width) {
        node_count = 0;
        edge_count = 0;
        write_beginning(out, evmdd.get_input(), evmdd.get_source_node());
        process_nodes(out, evmdd.get_source_node());
        write_alignment(out);
        // Write header to separate file if expression length > max_width
        if (arithmetic.size() > max_width) {
            std::ofstream information("info_" + filename);
            information << "graph info {";
            write_header(information, arithmetic, conditional, max_width);
            write_end(information);
        } else {
            write_header(out, arithmetic, conditional, max_width);
        }

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

    // Returns conditional effects as html-encoded string
    std::string encode_cond_effects(
        std::vector<std::string> const &conditional) const {
        if (conditional.empty()) {
            return "<tr><td>none</td></tr>";
        }
        std::stringstream ss;
        for (auto const &effect : conditional) {
            ss << "<tr><td align=\"center\">" << effect << "</td></tr>";
        }
        ss << "<tr><td></td></tr>";
        std::string effects = ss.str();
        // Html-encode string to prevent syntax errors
        // Replace -> in string to prevent error caused by ">"
        for (std::string::size_type pos = 0;
             (pos = effects.find("->")) != std::string::npos; pos += 4) {
            effects.replace(pos, 2, "&rArr;");
        }
        // Replace & sign with &amp if exists
        if (effects.find("&&") != std::string::npos) {
            effects = html_encode(effects);
        }
        return effects;
    }

    // Checks if string contains & sign which causes error in html
    // Returns string replacing & sign with &amp if there are any.
    std::string html_encode(std::string const &expression) const {
        if (expression.find("&") != std::string::npos) {
            std::string copy_expression = expression;
            for (std::string::size_type pos = 0;
                 (pos = copy_expression.find("&", pos)) != std::string::npos;
                 pos += 2) {
                copy_expression.replace(pos, 1, "&amp;");
            }
            return copy_expression;
        }
        return expression;
    }

    // Splits long expression into separate rows by <tr><td> html-tags
    std::string format_long_expression(std::string const &text,
                                       int &width) const {
        std::string copy_text = text;
        copy_text.insert(0, "<tr><td>");
        // Insert line breaks to text.
        // 30 chars per line. (Skip 18 chars to omit html tags)
        std::string::size_type pos = (width + 8);
        while (pos < copy_text.size()) {
            copy_text.insert(pos, "</td></tr><tr><td>");
            pos += (width + 18);
        }
        copy_text.append("</td></tr>");
        return html_encode(copy_text);
    }

    // Prints an informative header about EVMDD to the top
    void write_header(std::ostream &out, std::string const &arithmetic,
                      std::vector<std::string> const &conditional,
                      int max_width) const {
        out << "labelloc=\"t\";" << std::endl;
        out << "label=<<table cellborder=\"0\">";
        // Expressions
        out << "<tr><td border=\"1\" align=\"center\">";
        out << "Arithmetic Expression:</td></tr>";
        out << format_long_expression(arithmetic, max_width);
        out << "<tr><td border=\"1\" align=\"center\">";
        out << "Conditional Effects:</td></tr>";
        // Html-encode expression if it contains && operator
        out << encode_cond_effects(conditional);
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
