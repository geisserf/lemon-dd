#ifndef NUMERIC_CATAMORPH_OPTIMIZATIONS_H
#define NUMERIC_CATAMORPH_OPTIMIZATIONS_H

#include "../Catamorph.h"


class Optimizations {
private:
    template <typename Tag, typename Step>
    static Expression optimize_op(op<Tag, Expression> const &e, float neutral, Step step);
    template <typename Range>
    static bool has_zero(Range const &subs);
    static Expression opt_add_sub_alg(expression_r<Expression> const &e);
    static Expression opt_mul_div_alg(expression_r<Expression> const &e);

public:
    template <typename Tag>
    static void throw_wrong_parameters(op<Tag, Expression> const &e,
                                std::string expected);
    static void throw_division_by_zero();

    static Expression optimize_alg(expression_r<Expression> const &e);
};



#endif //NUMERIC_CATAMORPH_OPTIMIZATIONS_H
