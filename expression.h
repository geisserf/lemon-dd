#ifndef CATAMORPH_OP_H
#define CATAMORPH_OP_H
#include "boost_dependencies/boost/algorithm/string/join.hpp"
#include "boost_dependencies/boost/range/adaptors.hpp"
#include "boost_dependencies/boost/variant.hpp"
#include <string>
#include <vector>

using nb = float;
using id = std::string;

struct add_tag {};
struct sub_tag {};
struct mul_tag {};
struct div_tag {};
struct and_tag {};
struct equals_tag {};

template <typename Tag, typename R> struct op {
  op() = default;

  template <typename Range>
  explicit op(Range const &rng) : m_rands(rng.begin(), rng.end()) {}

  std::vector<R> const &rands() const { return m_rands; }

private:
  std::vector<R> m_rands;
};

template <typename R> using add_op = op<add_tag, R>;
template <typename R> using sub_op = op<sub_tag, R>;
template <typename R> using mul_op = op<mul_tag, R>;
template <typename R> using div_op = op<div_tag, R>;
template <typename R> using and_op = op<and_tag, R>;
template <typename R> using equals_op = op<equals_tag, R>;

template <typename R>
using expression_r = boost::variant<float, id, add_op<R>, mul_op<R>, sub_op<R>,
                                    div_op<R>, and_op<R>, equals_op<R>>;

struct expression : boost::recursive_wrapper<expression_r<expression>> {
  using boost::recursive_wrapper<expression_r<expression>>::recursive_wrapper;
};

#endif // CATAMORPH_OP_H
