
#ifndef CATAMORPH_CATAMORPHISM_H
#define CATAMORPH_CATAMORPHISM_H
#include "expression.h"
#include "functor_instance.h"

template <typename Out, typename Algebra>
Out cata(Algebra f, expression const &ast) {
  return f(fmap([f](expression const &e) -> Out { return cata<Out>(f, e); },
                ast.get()));
}

#endif // CATAMORPH_CATAMORPHISM_H