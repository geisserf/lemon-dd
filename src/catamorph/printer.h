#ifndef NUMERIC_CATAMORPH_PRINTER_H
#define NUMERIC_CATAMORPH_PRINTER_H

#include "catamorph.h"
#include "expression.h"
#include "factories.h"
#include <iomanip>
#include <iostream>
#include <string>

#include "../boost_dependencies/boost/algorithm/string/join.hpp"

class Printer {
private:
    template <typename Tag>
    static std::string print_op(op<Tag, std::string> const &e,
                                std::string const &op_repr) {
        return std::string("(") + op_repr + " " +
               boost::algorithm::join(e.rands(), " ") + ")";
    }

    static std::string print_alg(expression_r<std::string> const &e) {
        if (auto *o = Factories::get_as_add(e))
            return Printer::print_op(*o, "+");
        if (auto *o = Factories::get_as_sub(e))
            return Printer::print_op(*o, "-");
        if (auto *o = Factories::get_as_mul(e))
            return Printer::print_op(*o, "*");
        if (auto *o = Factories::get_as_div(e))
            return Printer::print_op(*o, "/");
        if (auto *o = Factories::get_as_equals(e))
            return Printer::print_op(*o, "==");
        if (auto *o = Factories::get_as_and(e))
            return Printer::print_op(*o, "^");
        if (auto *i = Factories::get_as_cst(e)) {
            // We use the stream operator to print e instead of std::to_string,
            // because then we can set the precision before we print (otherwise
            // even integer constant n is printed as n.000000
            std::ostringstream out;
            out << *i;
            return out.str();
        }
        if (auto *v = Factories::get_as_var(e))
            return *v;
        throw std::logic_error("Missing case in pattern matching");
    }

public:
    static void print(Expression const &e) {
        std::cout << Catamorph::cata<std::string>(Printer::print_alg, e)
                  << '\n';
    }

    static std::string asString(Expression const &e) {
        return Catamorph::cata<std::string>(Printer::print_alg, e);
    }
};

#endif // NUMERIC_CATAMORPH_PRINTER_H
