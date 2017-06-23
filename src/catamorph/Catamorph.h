#ifndef NUMERIC_CATAMORPH_CATAMORPH_H
#define NUMERIC_CATAMORPH_CATAMORPH_H
#include <string>
#include <vector>

#include "Factories.h"
#include "Expression.h"



class Catamorph {
public:
    template <typename Out, typename Algebra>
     static Out cata(Algebra f, Expression const &ast) {
      return f(fmap([f](Expression const &e) -> Out { return cata<Out>(f, e); },
                    ast.get()));
    }

    template<typename A, typename M>
    static auto fmap(M map, expression_r<A> const &e) {

      using B = decltype(map(std::declval<A>()));
      using Out = expression_r<B>;

      if (auto *o = Factories::get_as_add(e))
        return Out(add_op<B>(o->rands() | transformed(map)));

      if (auto *o = Factories::get_as_sub(e))
        return Out(sub_op<B>(o->rands() | transformed(map)));

      if (auto *o = Factories::get_as_mul(e))
        return Out(mul_op<B>(o->rands() | transformed(map)));

      if (auto *o = Factories::get_as_div(e))
        return Out(div_op<B>(o->rands() | transformed(map)));

      if (auto *o = Factories::get_as_and(e))
        return Out(and_op<B>(o->rands() | transformed(map)));

      if (auto *o = Factories::get_as_equals(e))
        return Out(equals_op<B>(o->rands() | transformed(map)));

      if (auto *i = Factories::get_as_cst(e))
        return Out(*i);
      if (auto *v = Factories::get_as_var(e))
        return Out(*v);

      throw std::logic_error("Missing case in pattern matching");
    }

};




#endif //NUMERIC_CATAMORPH_CATAMORPH_H
