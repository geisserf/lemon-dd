#ifndef LOGIC_AND_H
#define LOGIC_AND_H

#include <functional>

/**
 * Template class logic_and throws exception if parameters are 
 * none of the types int, float or double.
 * */
template <typename T> 
struct logic_and {
    T operator()(const T&, const T&) const {
        throw std::logic_error("Non-boolean operands for logic_and");
    }
};

/**
 * Checks if the operands are suitable for logic_and operation
 * */
#ifdef DEBUG
template<typename T> 
void domain_check(const T& lhs, const T& rhs){ 
    assert((lhs==0 || lhs==1));
    assert((rhs==0 || rhs==1));
    if(!((lhs==0||lhs==1)&&(rhs==0||rhs==1))) {
        throw std::logic_error("Domain size >2 for logic_and");
    }
}
#endif


template <>
struct logic_and <int> { 
    int operator()(const int& lhs, const int& rhs) const {
    #ifdef DEBUG
    domain_check(lhs,rhs);
    #endif
    auto result = std::logical_and<int>()(lhs, rhs);
    return result;
    }
};

template<>
struct logic_and <double> { 
    double operator()(const double& lhs, const double& rhs) const {
    #ifdef DEBUG
    domain_check(lhs,rhs);
    #endif
    auto result = std::logical_and<double>()(lhs, rhs);
    return result;
    }
};

template<>
struct logic_and <float> { 
    float operator()(const float& lhs, const float& rhs) const {
    #ifdef DEBUG
    domain_check(lhs,rhs);
    #endif
    auto result = std::logical_and<>()(lhs, rhs);
    return result;
    }
};


#endif /* LOGIC_AND_H */
