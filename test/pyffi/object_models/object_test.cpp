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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "pyffi/object_models/object.hpp"
#include "pyffi/exceptions.hpp"

using namespace boost;
using namespace pyffi;
using namespace pyffi::object_models;

BOOST_AUTO_TEST_SUITE(object_test_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
	// test default constructor
	BOOST_CHECK_NO_THROW(Object());

	// test constructor with various types
	Object obj_int(20513);
	BOOST_CHECK_EQUAL(get<int>(obj_int), 20513);
	Object obj_char('x');
	BOOST_CHECK_EQUAL(get<char>(obj_char), 'x');
	Object obj_str(std::string("Hello world!"));
	BOOST_CHECK_EQUAL(get<std::string>(obj_str), "Hello world!");

	// test copy constructor
	Object obj_int_copy(obj_int);
	BOOST_CHECK_EQUAL(get<int>(obj_int), get<int>(obj_int_copy));
	Object obj_char_copy(obj_char);
	BOOST_CHECK_EQUAL(get<char>(obj_char), get<char>(obj_char_copy));
	Object obj_str_copy(obj_str);
	BOOST_CHECK_EQUAL(get<std::string>(obj_str), get<std::string>(obj_str_copy));
}

BOOST_AUTO_TEST_CASE(get_test)
{
	// check that get returns assigned value
	Object obj(short(5));
	BOOST_CHECK_EQUAL(get<short>(obj), 5);

	// check bad type casts
	BOOST_CHECK_THROW(get<int>(obj), bad_get);
	BOOST_CHECK_THROW(get<float>(obj), bad_get);
	BOOST_CHECK_THROW(get<char>(obj), bad_get);
	BOOST_CHECK_THROW(get<std::string>(obj), bad_get);

	// check assignment via get
	get<short>(obj) = 10;
	BOOST_CHECK_EQUAL(get<short>(obj), 10);
}

BOOST_AUTO_TEST_CASE(assign_by_value_test)
{
	Object obj(50);

	// check assigning different value
	obj = 100;
	BOOST_CHECK_EQUAL(get<int>(obj), 100);

	// change type on assignment, and check new value and type
	BOOST_CHECK_NO_THROW(obj = 1.0f);
	BOOST_CHECK_EQUAL(get<float>(obj), 1.0f);
	BOOST_CHECK_NO_THROW(obj = 'x');
	BOOST_CHECK_EQUAL(get<char>(obj), 'x');
	BOOST_CHECK_NO_THROW(obj = "Hello world!");
	BOOST_CHECK_EQUAL(get<std::string>(obj), "Hello world!");
}

BOOST_AUTO_TEST_CASE(assign_by_object_test)
{
	Object obj(50);

	// check assigning different value
	obj = Object(100);
	BOOST_CHECK_EQUAL(get<int>(obj), 100);

	// change type on assignment
	BOOST_CHECK_NO_THROW(obj = Object(1.0f));
	BOOST_CHECK_EQUAL(get<float>(obj), 1.0f);
	BOOST_CHECK_NO_THROW(obj = Object('x'));
	BOOST_CHECK_EQUAL(get<char>(obj), 'x');
	BOOST_CHECK_NO_THROW(obj = Object("Hello world!"));
	BOOST_CHECK_EQUAL(get<std::string>(obj), "Hello world!");

	obj = Object(Object(101));
	BOOST_CHECK_EQUAL(get<int>(obj), 101);

	obj = 102;
	obj = Object(obj);
	BOOST_CHECK_EQUAL(get<int>(obj), 102);
}

BOOST_AUTO_TEST_CASE(type_string_test)
{
	Object obj(100);
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_type_string(), obj), "Int32");
	std::vector<Object> vec;
	vec.push_back(obj);
	vec.push_back(std::vector<Object>(3, Object('x')));
	vec.push_back(Object(1.0f));
	Object vecobj(vec);
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_type_string(), vecobj), "( Int32 ( Char Char Char ) Float )");
}

BOOST_AUTO_TEST_CASE(value_string_test)
{
	Object obj(100);
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), obj), "100");
	std::vector<Object> vec;
	vec.push_back(obj);
	vec.push_back(std::vector<Object>(3, Object('x')));
	vec.push_back(Object(1.1f));
	Object vecobj(vec);
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), vecobj), "( 100 ( x x x ) 1.1 )");
}

BOOST_AUTO_TEST_CASE(numeric_cast)
{
	Object obj;
	// simple bool cast
	obj = true;
	BOOST_CHECK_EQUAL(object_numeric_cast<bool>(obj), true);
	obj = false;
	BOOST_CHECK_EQUAL(object_numeric_cast<bool>(obj), false);
	obj = 1;
	BOOST_CHECK_EQUAL(object_numeric_cast<bool>(obj), true);
	obj = 0;
	BOOST_CHECK_EQUAL(object_numeric_cast<bool>(obj), false);
	obj = 2;
	BOOST_CHECK_THROW(object_numeric_cast<bool>(obj), boost::numeric::positive_overflow);
	obj = -1;
	BOOST_CHECK_THROW(object_numeric_cast<bool>(obj), boost::numeric::negative_overflow);
	// simple integer cast
	obj = 100;
	BOOST_CHECK_EQUAL(object_numeric_cast<int>(obj), 100);
	// simple float cast
	obj = 2.1;
	BOOST_CHECK_EQUAL(object_numeric_cast<int>(obj), 2);
	// cast from non-arithmetic type fails with type error
	obj = "hello world";
	BOOST_CHECK_THROW(object_numeric_cast<int>(obj), type_error);
	// check some 8 bit stuff
	obj = boost::int32_t(0xff);
	BOOST_CHECK_EQUAL(object_numeric_cast<boost::uint8_t>(obj), 0xff);
	BOOST_CHECK_THROW(object_numeric_cast<boost::int8_t>(obj), boost::numeric::positive_overflow);
	obj = boost::int32_t(0x100);
	BOOST_CHECK_THROW(object_numeric_cast<boost::uint8_t>(obj), boost::numeric::positive_overflow);
	BOOST_CHECK_THROW(object_numeric_cast<boost::int8_t>(obj), boost::numeric::positive_overflow);
	obj = boost::int32_t(0x7f);
	BOOST_CHECK_EQUAL(object_numeric_cast<boost::uint8_t>(obj), 0x7f);
	BOOST_CHECK_EQUAL(object_numeric_cast<boost::int8_t>(obj), 0x7f);
	obj = boost::int32_t(-0x80);
	BOOST_CHECK_THROW(object_numeric_cast<boost::uint8_t>(obj), boost::numeric::negative_overflow);
	BOOST_CHECK_EQUAL(object_numeric_cast<boost::int8_t>(obj), -0x80);
	obj = boost::int32_t(-0x81);
	BOOST_CHECK_THROW(object_numeric_cast<boost::uint8_t>(obj), boost::numeric::negative_overflow);
	BOOST_CHECK_THROW(object_numeric_cast<boost::int8_t>(obj), boost::numeric::negative_overflow);
	// check some 64 bit stuff
	obj = boost::uint64_t(0x7fffffffffffffffLL);
	BOOST_CHECK_EQUAL(object_numeric_cast<boost::int64_t>(obj), 0x7fffffffffffffffLL);
	obj = boost::uint64_t(0x8000000000000000LL);
	BOOST_CHECK_THROW(object_numeric_cast<boost::int64_t>(obj), boost::numeric::positive_overflow);
	// note: a double can represent 0x8000000000000000LL exactly
	BOOST_CHECK_EQUAL(object_numeric_cast<double>(obj), 0x8000000000000000LL);
}

BOOST_AUTO_TEST_SUITE_END()
