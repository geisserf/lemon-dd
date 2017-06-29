#ifndef NUMERIC_CATAMORPH_EVAOL_H
#define NUMERIC_CATAMORPH_EVAOL_H

#include "../Catamorph.h"

class Evaluate {
public:
    static float eval(Env const &env, Expression const &e);
    static auto partial_eval_alg(Env const &env);
    static Expression partial_eval(Env const &env, Expression const &e);
};

#endif // NUMERIC_CATAMORPH_EVAOL_H
