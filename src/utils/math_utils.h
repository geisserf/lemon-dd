#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>

class MathUtils {
public:
    static bool is_equal(float first, float second);

    // Arbitrary T don't have a NaN definition, therefore we return false
    template <typename T>
    static bool is_nan(T /*weight*/) {
        return false;
    }

    static bool is_nan(int weight) {
        return std::isnan(weight);
    }

    static bool is_nan(float weight) {
        return std::isnan(weight);
    }

    static bool is_nan(double weight) {
        return std::isnan(weight);
    }
};

#endif /* MATH_UTILS_H */

