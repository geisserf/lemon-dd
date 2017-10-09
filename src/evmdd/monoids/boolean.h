#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "../monoid.h"

using BoolMonoid = Monoid<bool, std::logical_or<bool>>;

#endif /* BOOLEAN_H */
