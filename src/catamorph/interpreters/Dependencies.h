#ifndef NUMERIC_CATAMORPH_DEPENDENCIES_H
#define NUMERIC_CATAMORPH_DEPENDENCIES_H


#include "../Expression.h"

class Dependency{
public:
    template<typename Tag>
    static std::set<ID> join_sets(op<Tag, std::set<ID>> const &op);
    static std::set<ID> dependencies_alg(expression_r<std::set<ID>> const &e);
    static std::set<ID> dependencies(Expression const &e);
};



#endif //NUMERIC_CATAMORPH_DEPENDENCIES_H
