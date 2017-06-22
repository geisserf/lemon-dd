#ifndef NUMERIC_CATAMORPH_OPTIMIZATIONS_H
#define NUMERIC_CATAMORPH_OPTIMIZATIONS_H

#include "../catamorph.h"


class optimizations {
private:
    template <typename Tag, typename Step>
    static expression optimize_op(op<Tag, expression> const &e, float neutral, Step step);
    template <typename Range>
    static bool has_zero(Range const &subs);
    static expression opt_add_sub_alg(expression_r<expression> const &e);
    static expression opt_mul_div_alg(expression_r<expression> const &e);

public:
    template <typename Tag>
    static void throw_wrong_parameters(op<Tag, expression> const &e,
                                std::string expected);
    static void throw_division_by_zero();

    static expression optimize_alg(expression_r<expression> const &e);
};



#endif //NUMERIC_CATAMORPH_OPTIMIZATIONS_H
