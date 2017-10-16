#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "conditional_effects.h"
#include "cxxopts.hpp"
#include "effect_parser.h"
#include "evmdd/printer.h"
#include "polynomial.h"
#include "evmdd/product_evmdd.h"

using Ordering = std::map<std::string, int>;
using Domain = std::map<std::string, unsigned int>;

struct domain_ordering {
    Domain domains;
    Ordering ordering;
};

domain_ordering parse_domain(std::string ordering_string) {
    Domain domains;
    Ordering ordering;
    std::string current_var;
    int begin = 0;
    unsigned int pos = 0;
    int current_ordering = 1;

    for (char &c : ordering_string) {
        if (c == ':')
            current_var = ordering_string.substr(begin, pos - begin);
        if (c == ',') {
            int b = begin + current_var.size() + 1;
            // std::cout<<"var: "<<current_var;
            std::string domain = ordering_string.substr(b, pos - b);
            // std::cout<<" domain: "<<domain<<std::endl;;
            begin = pos + 1;
            domains.insert(
                std::pair<std::string, int>(current_var, std::stoi(domain)));
            ordering.insert(
                std::pair<std::string, int>(current_var, current_ordering));
            current_ordering++;
        }
        if (pos + 1 == ordering_string.size()) {
            int b = begin + current_var.size() + 1;
            // std::cout<<"var: "<<current_var;
            std::string domain = ordering_string.substr(b, pos - b + 1);
            // std::cout<<" domain: "<<domain<<std::endl;;
            begin = pos + 1;
            domains.insert(
                std::pair<std::string, int>(current_var, std::stoi(domain)));
            ordering.insert(
                std::pair<std::string, int>(current_var, current_ordering));
            current_ordering++;
        }

        pos++;
    }

    domain_ordering o;
    o.domains = domains;
    o.ordering = ordering;

    return o;
}
template <typename M, typename F>
void create_dot(std::ostream &output_stream, Evmdd<M, F> const &evmdd,
                Ordering const &o) {
    DotPrinter<M, F> printer(o);
    printer.to_dot(output_stream, evmdd);
}

int main(int argc, char *argv[]) {
    cxxopts::Options options("Numeric Catamorph",
                             "Creates evmdds from numeric expressions, "
                             "conditional effects, and combinations therof");

    options.add_options()("t,type",
                          "Type of Evmdd (cst: costfunction, ce: conditional "
                          "effects in ENF, c:combined)",
                          cxxopts::value<std::string>())(
        "e,expression",
        "The expression[s] of the evmdd add -e twice for 2 expressions. "
        "conditional effects require the form (a->v)&(c->d)...",
        cxxopts::value<std::vector<std::string>>())(
        "d,domain",
        "Domains and Ordering of the variables Ordering is reverse of domain "
        "ordering",
        cxxopts::value<std::string>())("f,filename", "dot output filename",
                                       cxxopts::value<std::string>());

    options.parse(argc, argv);

    std::string type_;
    std::vector<std::string> expressions;
    std::string domain_str;
    std::string filename;

    if (options.count("f")) {
        filename = options["f"].as<std::string>();
    } else {
        std::cout << options.help() << std::endl;
        return -1;
    }

    if (options.count("e")) {
        expressions = options["e"].as<std::vector<std::string>>();
    } else {
        std::cout << options.help() << std::endl;
        return -1;
    }

    if (options.count("t")) {
        type_ = options["t"].as<std::string>();
    } else {
        std::cout << options.help() << std::endl;
        return -1;
    }

    if (type_ == "c" && expressions.size() != 2) {
        std::cout << options.help() << std::endl;
        std::cout << "combined evmdd requires 2 expressions: first conditional "
                     "effects,second cost function"
                  << std::endl;
        return -1;
    }

    if (options.count("d")) {
        domain_str = options["d"].as<std::string>();
    } else {
        std::cout << options.help() << std::endl;
        return -1;
    }

    domain_ordering d_o = parse_domain(domain_str);
    std::string dot_file = filename + ".dot";
    std::ofstream dot_stream(dot_file);
    if (type_ == "ce") {
        EffectParser parser;
        ConditionalEffects effects = parser.parse(expressions[0]);
        auto evmdd = effects.create_evmdd(d_o.domains, d_o.ordering);
        create_dot(dot_stream, evmdd, d_o.ordering);
    } else if (type_ == "cst") {
        Polynomial p = Polynomial(expressions[0]);
        Evmdd<double> evmdd = p.create_evmdd<double>(d_o.domains, d_o.ordering);
        create_dot(dot_stream, evmdd, d_o.ordering);
    } else if (type_ == "c") {
        EffectParser parser;
        ConditionalEffects effects = parser.parse(expressions[0]);
        auto effect_evmdd = effects.create_evmdd(d_o.domains, d_o.ordering);
        Polynomial p = Polynomial(expressions[1]);
        Evmdd<double> cost_evmdd =
            p.create_evmdd<double>(d_o.domains, d_o.ordering);
        ProductFactory<Facts, double, Union, std::plus<double>> factory;
        auto product_evmdd = factory.product(effect_evmdd, cost_evmdd);
        create_dot(dot_stream, product_evmdd, d_o.ordering);
    } else {
        std::cout << "unknown type" << std::endl;
    }

    return 0;
}
