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
    return e;
}

Expression Iverson::opt_and_alg(expression_r<Expression> const &e) {
    if (auto *op = Factories::get_as_and(e)) {
        if (op->rands().size() < 2) {
            std::ostringstream s;
            s << "wrong number of parameters (expected >=2 was"
              << op->rands().size() << std::endl;
            throw std::logic_error(s.str());
        }
        /*
                for (auto sub = op->rands().begin() + 1; sub !=
           op->rands().end(); sub++) {
                    if(auto *i = get_as_cst(sub->get())){

                    } else{
                        //one element is not yet 1|0
                        return e;
                    }
                }
        */
        if (auto *i = Factories::get_as_cst(op->rands()[0].get())) {
            if (auto *j = Factories::get_as_cst(op->rands()[1].get())) {
                if (*i == 1 && *j == 1) {
                    return Factories::cst(1);
                } else {
                    return Factories::cst(0);
                }
            }
        }
    }
    return e;
}

Expression Iverson::iverson_alg(expression_r<Expression> const &e) {
    return opt_or_alg(opt_and_alg(opt_equals_alg(e).get()).get());
}
