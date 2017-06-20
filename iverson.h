
#ifndef CATAMORPH_IVERSON_H
#define CATAMORPH_IVERSON_H

#include "expression.h"
#include "optimizations.h"





expression opt_equals_alg(expression_r<expression> const &e) {

    if (auto *op = get_as_equals(e)) {
        if (op->rands().size() != 2) {
            throw_wrong_parameters(*op,"2");
        }

        if (auto *i = get_as_cst(op->rands()[0].get())) {
            if (auto *j = get_as_cst(op->rands()[1].get())) {
                if (*i == *j) {
                    return cst(1);
                } else {
                    return cst(0);
                }
            }
        }
    }

    return e;
}

expression opt_or_alg(expression_r<expression> const &e) {
    return e;
}

expression opt_and_alg(expression_r<expression> const &e) {

    if (auto *op = get_as_and(e)) {
        if (op->rands().size() < 2) {
            throw_wrong_parameters(*op,">2");
        }
/*
        for (auto sub = op->rands().begin() + 1; sub != op->rands().end(); sub++) {
            if(auto *i = get_as_cst(sub->get())){

            } else{
                //one element is not yet 1|0
                return e;
            }
        }
*/
        if(auto *i = get_as_cst(op->rands()[0].get())){
            if(auto *j = get_as_cst(op->rands()[1].get())){
               if(*i == 1 && *j == 1){
                   return cst(1);
               } else {
                   return cst(0);
               }
            }
        }
    }
    return e;
}

expression iverson_alg(expression_r<expression> const &e) {
    //return e;
    return opt_or_alg(opt_and_alg(opt_equals_alg(e).get()).get());
}

#endif //CATAMORPH_IVERSON_H