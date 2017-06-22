#ifndef NUMERIC_CATAMORPH_FACTORIES_H
#define NUMERIC_CATAMORPH_FACTORIES_H

#include "expression.h"


class factories {
public:
    static expression cst(float i);
    static expression var(id id);
    static expression add(std::vector<expression> const &rands);
    static expression sub(std::vector<expression> const &rands);
    static expression mul(std::vector<expression> const &rands);
    static expression div(std::vector<expression> const &rands);
    static expression land(std::vector<expression> const &rands);
    static expression equals(std::vector<expression> const &rands);


    template<typename T>
    static float const *get_as_cst(expression_r<T> const &e) {
      return boost::get<float>(&e);
    }

    template<typename T>
    static id const *get_as_var(expression_r<T> const &e) {
      return (id *)boost::get<id>(&e);
    }

    template<typename T>
    static add_op<T> const *get_as_add(expression_r<T> const &e) {
      return boost::get<add_op<T>>(&e);
    }

    template<typename T>
    static sub_op<T> const *get_as_sub(expression_r<T> const &e) {
      return boost::get<sub_op<T>>(&e);
    }

    template<typename T>
    static mul_op<T> const *get_as_mul(expression_r<T> const &e) {
      return boost::get<mul_op<T>>(&e);
    }

    template<typename T>
    static div_op<T> const *get_as_div(expression_r<T> const &e) {
      return boost::get<div_op<T>>(&e);
    }

    template<typename T>
    static and_op<T> const *get_as_and(expression_r<T> const &e) {
      return boost::get<and_op<T>>(&e);
    }

    template<typename T>
    static equals_op<T> const *get_as_equals(expression_r<T> const &e) {
      return boost::get<equals_op<T>>(&e);
    }

};


#endif //NUMERIC_CATAMORPH_FACTORIES_H
