#ifndef NUMERIC_CATAMORPH_FACTORIES_H
#define NUMERIC_CATAMORPH_FACTORIES_H

#include "Expression.h"

class Factories {
public:
    static Expression cst(float i);
    static Expression var(ID id);
    static Expression add(std::vector<Expression> const &rands);
    static Expression sub(std::vector<Expression> const &rands);
    static Expression mul(std::vector<Expression> const &rands);
    static Expression div(std::vector<Expression> const &rands);
    static Expression land(std::vector<Expression> const &rands);
    static Expression equals(std::vector<Expression> const &rands);

    template <typename T>
    static float const *get_as_cst(expression_r<T> const &e) {
        return boost::get<float>(&e);
    }

    template <typename T>
    static ID const *get_as_var(expression_r<T> const &e) {
        return (ID *)boost::get<ID>(&e);
    }

    template <typename T>
    static add_op<T> const *get_as_add(expression_r<T> const &e) {
        return boost::get<add_op<T>>(&e);
    }

    template <typename T>
    static sub_op<T> const *get_as_sub(expression_r<T> const &e) {
        return boost::get<sub_op<T>>(&e);
    }

    template <typename T>
    static mul_op<T> const *get_as_mul(expression_r<T> const &e) {
        return boost::get<mul_op<T>>(&e);
    }

    template <typename T>
    static div_op<T> const *get_as_div(expression_r<T> const &e) {
        return boost::get<div_op<T>>(&e);
    }

    template <typename T>
    static and_op<T> const *get_as_and(expression_r<T> const &e) {
        return boost::get<and_op<T>>(&e);
    }

    template <typename T>
    static equals_op<T> const *get_as_equals(expression_r<T> const &e) {
        return boost::get<equals_op<T>>(&e);
    }
};

#endif // NUMERIC_CATAMORPH_FACTORIES_H
