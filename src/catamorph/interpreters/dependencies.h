#ifndef NUMERIC_CATAMORPH_DEPENDENCIES_H
#define NUMERIC_CATAMORPH_DEPENDENCIES_H


#include "../expression.h"

class dependency{
public:
    template<typename Tag>
    static std::set<id> join_sets(op<Tag, std::set<id>> const &op);
    static std::set<id> dependencies_alg(expression_r<std::set<id>> const &e);
    static std::set<id> dependencies(expression const &e);
};



#endif //NUMERIC_CATAMORPH_DEPENDENCIES_H
