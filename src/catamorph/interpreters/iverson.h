#ifndef NUMERIC_CATAMORPH_IVERSON_H
#define NUMERIC_CATAMORPH_IVERSON_H


#include "../catamorph.h"

class iverson {
public:
    static expression opt_equals_alg(expression_r<expression> const &e);
    static expression opt_or_alg(expression_r<expression> const &e);
    static expression opt_and_alg(expression_r<expression> const &e);
    static expression iverson_alg(expression_r<expression> const &e);
};


#endif //NUMERIC_CATAMORPH_IVERSON_H
