#include "conditional_effects.h"
#include "effect_parser.h"
#include "evmdd/abstract_factory.h"
#include "evmdd/printer.h"
#include "globals.h"
#include "polynomial.h"
#include "utils/string_utils.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

Domains parse_domains(string const &domains) {
    Domains result;
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
    result.reserve(variables.size());
    for (string const &var : variables) {
        result.push_back(var);
    }
    std::reverse(result.begin(), result.end());
    return result;
}

template <typename M, typename F>
void create_dot(std::ostream &output_stream, std::string const &filename,
                Evmdd<M, F> const &evmdd, string const &arithmetic,
                vector<string> const &conditional) {
    DotPrinter<M, F> printer;
    printer.to_dot(output_stream, filename, evmdd, arithmetic, conditional, 80);
}

Evmdd<Facts, Union> generate_effect_evmdd(vector<string> const &effects,
                                          Domains const &domain,
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
    Domains domain = parse_domains(domains);

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

    std::chrono::time_point<std::chrono::system_clock> start, end;
    Evmdd<double> cost_evmdd;
    if (!arithmetic_expression.empty()) {
        cout << "Generating EVMDD for arithmetic function "
             << arithmetic_expression << endl;
        start = std::chrono::system_clock::now();
        Polynomial p = Polynomial(arithmetic_expression);
        cost_evmdd = p.create_evmdd<double>(domain, ordering);
        end = std::chrono::system_clock::now();
        int elapsed_seconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
        cout << "Finished after " << elapsed_seconds << "ms." << endl;
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

    Evmdd<Facts, Union> effect_evmdd;
    if (!conditional_effects.empty()) {
        effect_evmdd =
            generate_effect_evmdd(conditional_effects, domain, ordering);
    }

    if (arithmetic_expression.empty() && conditional_effects.empty()) {
        std::cerr << "User error: Both types of expressions are empty." << endl;
        return 0;
    }

    cout << "Enter name of result file." << endl;
    string filename;
    getline(cin, filename);
    if (filename.empty()) {
        // Set filename to current date-time
        time_t t = time(0);
        struct tm *now = localtime(&t);
        char buffer[80];
        strftime(buffer, 80, "%Y_%m_%d_%I%M%S%p", now);
        cout << "File name not entered. Setting to " << buffer << endl;
        filename = buffer;
    }
    filename += ".dot";
    std::ofstream dot_stream(filename);

    if (!cost_evmdd.exists()) {
        cout << "Writing conditional effect EVMDD to " << filename << endl;
        create_dot(dot_stream, filename, effect_evmdd, arithmetic_expression,
                   conditional_effects);
        return 0;
    }
    if (!effect_evmdd.exists()) {
        cout << "Writing arithmetic expression EVMDD to " << filename << endl;
        create_dot(dot_stream, filename, cost_evmdd, arithmetic_expression,
                   conditional_effects);
        return 0;
    }
    // Both EVMDDs were requested -> generate product EVMDD
    auto &factory =
        AbstractProductFactory<Facts, double, Union,
                               std::plus<double>>::get_factory(ordering,
                                                               domain);

    auto product_evmdd = factory.product(effect_evmdd, cost_evmdd);
    cout << "Both EVMDD types requested: Writing product EVMDD." << endl;
    create_dot(dot_stream, filename, product_evmdd, arithmetic_expression,
               conditional_effects);
}
