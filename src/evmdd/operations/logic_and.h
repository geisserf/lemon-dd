#ifndef LOGIC_AND_H
#define LOGIC_AND_H

#include <functional>

template <typename T> 
struct logic_and {
    T operator()(const T& lhs, const T& rhs) const {
        if(!((lhs==0 || lhs==1) && (rhs==0 || rhs==1))){
            throw std::logic_error("Non-boolean operands for logic_and");
        }
    }
};

template <>
struct logic_and <int> { 
    int operator()(const int& lhs, const int& rhs) const {
    assert((lhs==0 || lhs==1));
    assert((rhs==0 || rhs==1));
    auto result = std::logical_and<int>()(lhs, rhs);
    return result;
    }
};

template<>
struct logic_and <double> { 
    double operator()(const double& lhs, const double& rhs) const {
    assert((lhs==0 || lhs==1));
    assert((rhs==0 || rhs==1));
    auto result = std::logical_and<double>()(lhs, rhs);
    return result;
    }
};

template<>
struct logic_and <float> { 
    float operator()(const float& lhs, const float& rhs) const {
    assert((lhs==0 || lhs==1));
    assert((rhs==0 || rhs==1));
    auto result = std::logical_and<>()(lhs, rhs);
    return result;
    }
};


#endif /* LOGIC_AND_H */
