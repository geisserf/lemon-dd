//
// Created by bwright on 22.06.17.
//

#include "iverson.h"
#include "optimizations.h"

Expression Iverson::opt_equals_alg(expression_r<Expression> const &e) {
    if (auto *op = Factories::get_as_equals(e)) {
        if (op->rands().size() != 2) {
            std::ostringstream s;
            s << "wrong number of parameters (expected =2 was"
              << op->rands().size() << std::endl;
            throw std::logic_error(s.str());
        }

        if (auto *i = Factories::get_as_cst(op->rands()[0].get())) {
            if (auto *j = Factories::get_as_cst(op->rands()[1].get())) {
                if (*i == *j) {
                    return Factories::cst(1);
                } else {
                    return Factories::cst(0);
                }
            }
        }
    }

    return e;
}

Expression Iverson::opt_or_alg(expression_r<Expression> const &e) {
    if (auto *op = Factories::get_as_or(e)) {
        if (op->rands().size() != 2) {
            std::ostringstream s;
            s << "wrong number of parameters (expected >=2 was"
              << op->rands().size() << std::endl;
            throw std::logic_error(s.str());
        }
        if (auto *i = Factories::get_as_cst(op->rands()[0].get())) {
            if (auto *j = Factories::get_as_cst(op->rands()[1].get())) {
                int or_result = static_cast<int>(*i || *j);
                return Factories::cst(or_result);
            }
        }
    }

    return e;
}

Expression Iverson::opt_not_alg(expression_r<Expression> const &e) {
    if (auto *op = Factories::get_as_not(e)) {
        if (op->rands().size() != 1) {
            std::ostringstream s;
            s << "wrong number of parameters (expected 1 was"
              << op->rands().size() << std::endl;
            throw std::logic_error(s.str());
        }
        if (auto *i = Factories::get_as_cst(op->rands()[0].get())) {
            return Factories::cst(1 - *i);
        }
    }

    return e;
}

Expression Iverson::opt_and_alg(expression_r<Expression> const &e) {
    if (auto *op = Factories::get_as_and(e)) {
        if (op->rands().size() != 2) {
            std::ostringstream s;
            s << "wrong number of parameters (expected >=2 was"
              << op->rands().size() << std::endl;
            throw std::logic_error(s.str());
        }
        if (auto *i = Factories::get_as_cst(op->rands()[0].get())) {
            if (auto *j = Factories::get_as_cst(op->rands()[1].get())) {
                int and_result = static_cast<int>(*i && *j);
                return Factories::cst(and_result);
            }
        }
    }

    return e;
}

Expression Iverson::iverson_alg(expression_r<Expression> const &e) {
    return opt_not_alg(
        opt_or_alg(opt_and_alg(opt_equals_alg(e).get()).get()).get());
}
