#ifndef NUMERIC_CATAMORPH_EVAOL_H
#define NUMERIC_CATAMORPH_EVAOL_H


#include "../catamorph.h"

class evaluate {
public:
    static float eval(env const &env, expression const &e);
    static auto partial_eval_alg(env const &env);
    static expression partial_eval(env const &env, expression const &e);
};


#endif //NUMERIC_CATAMORPH_EVAOL_H
