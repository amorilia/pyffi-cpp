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

#ifndef PYFFI_OM_OBJECT_HPP_INCLUDED
#define PYFFI_OM_OBJECT_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/numeric/conversion/cast.hpp> // numeric_cast
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp> // enable_if, disable_if
#include <boost/variant.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "pyffi/exceptions.hpp"

namespace pyffi
{

namespace object_models
{

//! A boost::variant which can contain all of the usual types. All
//! data has this type.
typedef boost::make_recursive_variant<boost::int8_t, boost::uint8_t, boost::int16_t, boost::uint16_t, boost::int32_t, boost::uint32_t, boost::int64_t, boost::uint64_t, float, double, char, std::string,  std::vector<boost::recursive_variant_> >::type Object;

//! A visitor for getting the type as string.
class object_type_string
	: public boost::static_visitor<std::string>
{
public:
	std::string operator()(const boost::int8_t & value) const {
		return "Int8";
	};
	std::string operator()(const boost::uint8_t & value) const {
		return "UInt8";
	};
	std::string operator()(const boost::int16_t & value) const {
		return "Int16";
	};
	std::string operator()(const boost::uint16_t & value) const {
		return "UInt16";
	};
	std::string operator()(const boost::int32_t & value) const {
		return "Int32";
	};
	std::string operator()(const boost::uint32_t & value) const {
		return "UInt32";
	};
	std::string operator()(const boost::int64_t & value) const {
		return "Int64";
	};
	std::string operator()(const boost::uint64_t & value) const {
		return "UInt64";
	};
	std::string operator()(const float & value) const {
		return "Float";
	};
	std::string operator()(const double & value) const {
		return "Double";
	};
	std::string operator()(const char & value) const {
		return "Char";
	};
	std::string operator()(const std::string & value) const {
		return "String";
	};
	std::string operator()(const std::vector<Object> & value) const {
		std::string result = "( ";
		BOOST_FOREACH(const Object & obj, value) {
			result += boost::apply_visitor(object_type_string(), obj);
			result += " ";
		};
		result += ")";
		return result;
	};
};

//! A visitor for getting the value as string.
class object_value_string
	: public boost::static_visitor<std::string>
{
public:
	template <typename T>
	std::string operator()(const T & value) const {
		std::ostringstream o;
		if (!(o << value))
			throw runtime_error("object_value_string failed");
		return o.str();
	};
	std::string operator()(const std::vector<Object> & value) const {
		std::string result = "( ";
		BOOST_FOREACH(const Object & obj, value) {
			result += boost::apply_visitor(object_value_string(), obj);
			result += " ";
		};
		result += ")";
		return result;
	};
};

//! A visitor for getting an arithmetic type (bool, integer, float), with
//! range checking.
template <typename ValueType>
class object_numeric_cast
	: public boost::static_visitor<ValueType>
{
public:
	template <typename T>
	typename boost::enable_if<boost::is_arithmetic<T>, ValueType>::type
	operator()(const T & value) const {
		return boost::numeric_cast<ValueType>(value);
	};
	template <typename T>
	typename boost::disable_if<boost::is_arithmetic<T>, ValueType>::type
	operator()(const T & value) const {
		throw type_error(
		    "cannot numeric_cast " + std::string(typeid(T).name())
		    + " to " + std::string(typeid(ValueType).name()));
	};
};

}; // namespace object_models

}; // namespace pyffi

#endif
