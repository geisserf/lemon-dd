#include "math_utils.h"

#include <cmath>
#include <limits>

bool MathUtils::is_equal(float first, float second) {
    return std::fabs(first - second) <= std::numeric_limits<float>::epsilon();
}
