#include "conditional_effects.h"
#include "effect_parser.h"
#include "evmdd/abstract_factory.h"
#include "evmdd/printer.h"
#include "polynomial.h"
#include "utils/string_utils.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::vector;

using Ordering = std::map<std::string, int>;
using Domain = std::map<std::string, unsigned int>;

Domain parse_domains(string const &domains) {
    Domain result;
    std::istringstream iss(domains);
    vector<string> tokens{std::istream_iterator<string>{iss},
                          std::istream_iterator<string>{}};
    for (string const &token : tokens) {
        vector<string> var_domain_pair = StringUtils::split(token, ':');
        assert(var_domain_pair.size() == 2);
        result[var_domain_pair[0]] = std::stoi(var_domain_pair[1]);
    }
    return result;
}

Ordering parse_ordering(string const &ordering) {
    Ordering result;
    vector<string> variables = StringUtils::split(ordering, ' ');
    int position = variables.size() - 1; // position 0 corresponds to bottom var
    for (string const &var : variables) {
        result[var] = position--;
    }
    return result;
}

template <typename M, typename F>
void create_dot(std::ostream &output_stream, Evmdd<M, F> const &evmdd) {
    DotPrinter<M, F> printer;
    printer.to_dot(output_stream, evmdd);
}

Evmdd<Facts, Union> generate_effect_evmdd(vector<string> const &effects,
                                          Domain const &domain,
                                          Ordering const &ordering) {
    EffectParser parser;
    vector<ConditionalEffect> ce;
    for (string const &effect : effects) {
        ce.push_back(parser.parse_single_effect(effect));
    }
    return ConditionalEffects::create_evmdd(ce, domain, ordering);
}

int main() {
    cout << "Welcome to LEMon. This executable generates EVMDDs for arithmetic "
            "or logical expressions (as conditional effects) and saves them in "
            ".dot format."
         << endl;

    cout << "Enter variable domain size in the following form: <var1>:<size> "
            "... <var_n>:<size>"
         << endl;
    cout << "Example: a:2 b:4 c:3 d:4" << endl;
    string domains;
    getline(cin, domains);
    Domain domain = parse_domains(domains);

    cout << "Enter top-down ordering relation between variables in the "
            "following form: <var_i> <var_j> ... <var_k>."
         << endl;
    cout << "Example: b c d e a" << endl;
    string ordering_as_string;
    getline(cin, ordering_as_string);
    Ordering ordering = parse_ordering(ordering_as_string);

    cout << "Enter an arithmetic expression (press return to skip). For more "
            "information on arithmetic expressions we refer to the Wiki."
         << endl;
    string arithmetic_expression;
    getline(cin, arithmetic_expression);

    Evmdd<double> cost_evmdd;
    if (!arithmetic_expression.empty()) {
        Polynomial p = Polynomial(arithmetic_expression);
        cost_evmdd = p.create_evmdd<double>(domain, ordering);
    }

    cout << "Enter conditional effects. Press return once finished. For more "
            "information on conditional effects we refer to the Wiki."
         << endl;

    string conditional_effect;
    vector<string> conditional_effects;
    while (getline(cin, conditional_effect)) {
        if (conditional_effect == "") {
            break;
        }
        conditional_effects.push_back(conditional_effect);
    }
    Evmdd<Facts, Union> effect_evmdd =
        generate_effect_evmdd(conditional_effects, domain, ordering);

    if (arithmetic_expression.empty() && conditional_effects.empty()) {
        std::cerr << "User error: Both types of expressions are empty." << endl;
        return 0;
    }

    cout << "Enter name of result file." << endl;
    string filename;
    getline(cin, filename);
    std::ofstream dot_stream(filename);

    if (!cost_evmdd.exists()) {
        cout << "Writing conditional effect EVMDD." << endl;
        create_dot(dot_stream, effect_evmdd);
        return 0;
    }
    if (!effect_evmdd.exists()) {
        cout << "Writing arithmetic expression EVMDD." << endl;
        create_dot(dot_stream, cost_evmdd);
        return 0;
    }
    // Both EVMDDs were requested -> generate product EVMDD
    auto &factory =
        AbstractProductFactory<Facts, double, Union,
                               std::plus<double>>::get_factory(ordering);

    auto product_evmdd = factory.product(effect_evmdd, cost_evmdd);
    cout << "Both EVMDD types requested: Writing product EVMDD." << endl;
    create_dot(dot_stream, product_evmdd);
}

// int main(int argc, char *argv[]) {
//    cxxopts::Options options("Numeric Catamorph",
//                             "Creates evmdds from numeric expressions, "
//                             "conditional effects, and combinations therof");
//
//    options.add_options()("t,type",
//                          "Type of Evmdd (cst: costfunction, ce: conditional "
//                          "effects in ENF, c:combined)",
//                          cxxopts::value<std::string>())(
//        "e,expression",
//        "The expression[s] of the evmdd add -e twice for 2 expressions. "
//        "conditional effects require the form (a->v)&(c->d)...",
//        cxxopts::value<std::vector<std::string>>())(
//        "d,domain",
//        "Domains and Ordering of the variables Ordering is reverse of domain "
//        "ordering",
//        cxxopts::value<std::string>())("f,filename", "dot output filename",
//                                       cxxopts::value<std::string>());
//
//    options.parse(argc, argv);
//
//    std::string type_;
//    std::vector<std::string> expressions;
//    std::string domain_str;
//    std::string filename;
//
//    if (options.count("f")) {
//        filename = options["f"].as<std::string>();
//    } else {
//        std::cout << options.help() << std::endl;
//        return -1;
//    }
//
//    if (options.count("e")) {
//        expressions = options["e"].as<std::vector<std::string>>();
//    } else {
//        std::cout << options.help() << std::endl;
//        return -1;
//    }
//
//    if (options.count("t")) {
//        type_ = options["t"].as<std::string>();
//    } else {
//        std::cout << options.help() << std::endl;
//        return -1;
//    }
//
//    if (type_ == "c" && expressions.size() != 2) {
//        std::cout << options.help() << std::endl;
//        std::cout << "combined evmdd requires 2 expressions: first conditional
//        "
//                     "effects,second cost function"
//                  << std::endl;
//        return -1;
//    }
//
//    if (options.count("d")) {
//        domain_str = options["d"].as<std::string>();
//    } else {
//        std::cout << options.help() << std::endl;
//        return -1;
//    }
//
//    domain_ordering d_o = parse_domain(domain_str);
//    std::string dot_file = filename + ".dot";
//    std::ofstream dot_stream(dot_file);
//    if (type_ == "ce") {
//        EffectParser parser;
//        std::vector<ConditionalEffect> effects = parser.parse(expressions[0]);
//        auto evmdd = ConditionalEffects::create_evmdd(effects, d_o.domains,
//                                                      d_o.ordering);
//        create_dot(dot_stream, evmdd, d_o.ordering);
//    } else if (type_ == "cst") {
//        Polynomial p = Polynomial(expressions[0]);
//        Evmdd<double> evmdd = p.create_evmdd<double>(d_o.domains,
//        d_o.ordering);
//        create_dot(dot_stream, evmdd, d_o.ordering);
//    } else if (type_ == "c") {
//        EffectParser parser;
//        std::vector<ConditionalEffect> effects = parser.parse(expressions[0]);
//        auto effect_evmdd = ConditionalEffects::create_evmdd(
//            effects, d_o.domains, d_o.ordering);
//        Polynomial p = Polynomial(expressions[1]);
//        Evmdd<double> cost_evmdd =
//            p.create_evmdd<double>(d_o.domains, d_o.ordering);
//        auto &factory = AbstractProductFactory<
//            Facts, double, Union,
//            std::plus<double>>::get_factory(d_o.ordering);
//
//        auto product_evmdd = factory.product(effect_evmdd, cost_evmdd);
//        create_dot(dot_stream, product_evmdd, d_o.ordering);
//    } else {
//        std::cout << "unknown type" << std::endl;
//    }
//
//    return 0;
//}
