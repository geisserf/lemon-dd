#ifndef NUMERIC_CATAMORPH_EXPRESSION_H
#define NUMERIC_CATAMORPH_EXPRESSION_H

#include "../boost_dependencies/boost/range/adaptors.hpp"
#include "../boost_dependencies/boost/variant.hpp"

using NBR = float;
using ID = std::string;
using Env = std::map<ID, NBR>;

struct add_tag {};
struct sub_tag {};
struct mul_tag {};
struct div_tag {};
struct and_tag {};
struct equals_tag {};

template <typename Tag, typename R>
struct op {
    op() = default;

    template <typename Range>
    explicit op(Range const &rng) : m_rands(rng.begin(), rng.end()) {}

    std::vector<R> const &rands() const {
        return m_rands;
    }

private:
    std::vector<R> m_rands;
};

template <typename R>
using add_op = op<add_tag, R>;
template <typename R>
using sub_op = op<sub_tag, R>;
template <typename R>
using mul_op = op<mul_tag, R>;
template <typename R>
using div_op = op<div_tag, R>;
template <typename R>
using and_op = op<and_tag, R>;
template <typename R>
using equals_op = op<equals_tag, R>;

template <typename R>
using expression_r = boost::variant<float, ID, add_op<R>, mul_op<R>, sub_op<R>,
                                    div_op<R>, and_op<R>, equals_op<R>>;

struct Expression : boost::recursive_wrapper<expression_r<Expression>> {
    using boost::recursive_wrapper<expression_r<Expression>>::recursive_wrapper;
};

#endif // NUMERIC_CATAMORPH_EXPRESSION_H
