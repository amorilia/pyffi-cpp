/*

Copyright (c) 2007-2008, Python File Format Interface
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the Python File Format Interface
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef PYFFI_OM_EXPRESSION_HPP_INCLUDED
#define PYFFI_OM_EXPRESSION_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/variant.hpp>

#include "pyffi/object_models/object.hpp"

using boost::apply_visitor;

namespace pyffi
{

namespace object_models
{

struct op_logical_or;
struct op_logical_and;
struct op_logical_not;
struct op_eq;
struct op_neq;
struct op_gt;
struct op_lt;
struct op_gteq;
struct op_lteq;
struct op_bitwise_or;
struct op_bitwise_and;
struct op_leftshift;
struct op_rightshift;
struct op_plus;
struct op_minus;
struct op_negate;
struct op_bitwise_not;
struct op_multiply;
struct op_divide;
struct op_modulo;
struct op_power;

template <typename operator_tag> struct binary_operator;
template <typename operator_tag> struct unary_operator;
struct index_operator;
struct call_operator;
struct member_operator;

// implementation note: using mpl::vector to get around maximum of 20
// template parameters

typedef boost::mpl::vector<
Object,
boost::recursive_wrapper<binary_operator<op_logical_or> >,
boost::recursive_wrapper<binary_operator<op_logical_and> >,
boost::recursive_wrapper<unary_operator<op_logical_not> >,
boost::recursive_wrapper<binary_operator<op_eq> >,
boost::recursive_wrapper<binary_operator<op_eq> >,
boost::recursive_wrapper<binary_operator<op_neq> >,
boost::recursive_wrapper<binary_operator<op_gt> >,
boost::recursive_wrapper<binary_operator<op_lt> >,
boost::recursive_wrapper<binary_operator<op_gteq> >,
boost::recursive_wrapper<binary_operator<op_lteq> >,
boost::recursive_wrapper<binary_operator<op_bitwise_or> >,
boost::recursive_wrapper<binary_operator<op_bitwise_and> >,
boost::recursive_wrapper<binary_operator<op_leftshift> >,
boost::recursive_wrapper<binary_operator<op_rightshift> >,
boost::recursive_wrapper<binary_operator<op_plus> >,
boost::recursive_wrapper<binary_operator<op_minus> >,
boost::recursive_wrapper<unary_operator<op_negate> >,
boost::recursive_wrapper<binary_operator<op_bitwise_not> >,
boost::recursive_wrapper<binary_operator<op_multiply> >
>::type _expr_types_1;

typedef boost::mpl::vector<
boost::recursive_wrapper<binary_operator<op_divide> >,
      boost::recursive_wrapper<binary_operator<op_modulo> >,
      boost::recursive_wrapper<binary_operator<op_power> >,
      boost::recursive_wrapper<call_operator>,
      boost::recursive_wrapper<index_operator>,
      boost::recursive_wrapper<member_operator>
      >::type _expr_types_2;

typedef boost::mpl::insert_range<
_expr_types_1,
boost::mpl::end<_expr_types_1>::type,
_expr_types_2>::type
_expr_types;

//! A boost::variant which can contain any expression. All
//! expressions have this type.
typedef boost::make_variant_over<_expr_types>::type Expression;

template <typename operator_tag>
struct binary_operator {
	Expression lhs, rhs;
	binary_operator(const Expression & lhs, const Expression & rhs)
		: lhs(lhs), rhs(rhs) {};
};

template <typename operator_tag>
struct unary_operator {
	Expression rhs;
	unary_operator(const Expression & rhs) : rhs(rhs) {};
};

struct index_operator {
	std::vector<Expression> indices;
	index_operator(const std::vector<Expression> & indices) : indices(indices) {};
};

struct call_operator {};

struct member_operator {};

//! A visitor for evaluating an expression to an object. The template
//! argument IntegerType determines which integer type is used for
//! intermediate calculations.
template <typename IntegerType>
class expression_object_visitor
	: public boost::static_visitor<Object>
{
private:
	IntegerType numeric_cast(const Expression & value) const {
		return object_numeric_cast<IntegerType>(boost::apply_visitor(expression_object_visitor<IntegerType>(), value));
	};
public:
	template <typename T>
	Object operator()(const T & value) const {
		throw runtime_error("expression_object not yet implemented for " + std::string(typeid(T).name()) + ".");
	};
	Object operator()(const Object & value) const {
		return value;
	};
	/*
	Object operator()(const binary_operator<op_logical_or> & value) const {
	};
	Object operator()(const binary_operator<op_logical_and> & value) const {
	};
	Object operator()(const binary_operator<op_logical_not> & value) const {
	};
	Object operator()(const binary_operator<op_eq> & value) const {
	};
	Object operator()(const binary_operator<op_neq> & value) const {
	};
	Object operator()(const binary_operator<op_gt> & value) const {
	};
	Object operator()(const binary_operator<op_lt> & value) const {
	};
	Object operator()(const binary_operator<op_gteq> & value) const {
	};
	Object operator()(const binary_operator<op_lteq> & value) const {
	};
	Object operator()(const binary_operator<op_bitwise_or> & value) const {
	};
	Object operator()(const binary_operator<op_bitwise_and> & value) const {
	};
	Object operator()(const binary_operator<op_leftshift> & value) const {
	};
	Object operator()(const binary_operator<op_rightshift> & value) const {
	};
	*/
	Object operator()(const binary_operator<op_plus> & value) const {
		return numeric_cast(value.lhs) + numeric_cast(value.rhs);
	};
	/*
	Object operator()(const binary_operator<op_minus> & value) const {
	};
	Object operator()(const unary_operator<op_negate> & value) const {
	};
	*/
};

//! Evaluating an expression to an object. The template argument
//! IntegerType determines which integer type is used for intermediate
//! calculations.
template <typename IntegerType>
Object expression_object(const Expression & exp)
{
	return boost::apply_visitor(expression_object_visitor<IntegerType>(), exp);
};

//! Evaluate an expression to a numeric value. The template argument
//! IntegerType determines which integer type is used for intermediate
//! calculations.
template <typename IntegerType, typename ValueType>
ValueType expression_numeric_cast(const Expression & exp)
{
	return object_numeric_cast<ValueType>(expression_object<IntegerType>(exp));
};

} // object_models

} // pyffi

#endif // PYFFI_OM_EXPRESSION_HPP_INCLUDED
