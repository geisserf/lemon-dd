#ifndef NUMERIC_CATAMORPH_IVERSON_H
#define NUMERIC_CATAMORPH_IVERSON_H

#include "../catamorph.h"

class Iverson {
public:
    static Expression opt_not_alg(expression_r<Expression> const &e);
    static Expression opt_equals_alg(expression_r<Expression> const &e);
    static Expression opt_or_alg(expression_r<Expression> const &e);
    static Expression opt_and_alg(expression_r<Expression> const &e);
    static Expression iverson_alg(expression_r<Expression> const &e);
};

#endif // NUMERIC_CATAMORPH_IVERSON_H
