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

#include "pyffi/object_models/int_type.hpp"
#include "pyffi/exceptions.hpp"

using namespace pyffi;
using namespace pyffi::object_models;

BOOST_AUTO_TEST_SUITE(int_type_test_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
	// test default constructor
	IntType<uint8_t> i;
	BOOST_CHECK_EQUAL(i.get(), 0);

	// test copy constructor
	i = 10;
	IntType<uint8_t> j(i);
	BOOST_CHECK_EQUAL(i.get(), j.get());
}

BOOST_AUTO_TEST_CASE(get_test)
{
	// check that get returns assigned value
	IntType<uint8_t> i;
	i = 5;
	BOOST_CHECK_EQUAL(i.get(), 5);

	// check changing assignment via get
	i.get() = 6;
	BOOST_CHECK_EQUAL(i.get(), 6);
}

BOOST_AUTO_TEST_CASE(assign_by_value_test)
{
	IntType<uint8_t> i;

	// test assignment of correct type
	i = (uint8_t)5;
	BOOST_CHECK_EQUAL(i.get(), 5);

	// check assigning different value
	i = (uint8_t)100;
	BOOST_CHECK_EQUAL(i.get(), 100);

	// type is converted on assignment
	i = (int64_t)3;
	BOOST_CHECK_EQUAL(i.get(), 3);
	i = 'x';
	BOOST_CHECK_EQUAL(i.get(), 0x78); // chr(0x78) is 'x'

	// casting to incompatible type fails
	BOOST_CHECK_THROW(i = 1.0f, type_error);
}

BOOST_AUTO_TEST_CASE(range_check_uint8_test)
{
	// range check error
	IntType<uint8_t> i;
	BOOST_CHECK_THROW(i = -1, value_error);
	i = 0;
	BOOST_CHECK_EQUAL(i.get(), 0);
	i = 0xff;
	BOOST_CHECK_EQUAL(i.get(), 0xff);
	BOOST_CHECK_THROW(i = 0x100, value_error);
}

BOOST_AUTO_TEST_CASE(range_check_uint16_test)
{
	IntType<uint16_t> i;
	BOOST_CHECK_THROW(i = -1, value_error);
	i = 0;
	BOOST_CHECK_EQUAL(i.get(), 0);
	i = 0xffff;
	BOOST_CHECK_EQUAL(i.get(), 0xffff);
	BOOST_CHECK_THROW(i = 0x10000, value_error);
}

BOOST_AUTO_TEST_CASE(range_check_uint32_test)
{
	IntType<uint32_t> i;
	BOOST_CHECK_THROW(i = -1, value_error);
	i = 0;
	BOOST_CHECK_EQUAL(i.get(), 0);
	i = 0xffffffff;
	BOOST_CHECK_EQUAL(i.get(), 0xffffffff);
	BOOST_CHECK_THROW(i = 0x100000000, value_error);
}

BOOST_AUTO_TEST_CASE(range_check_uint64_test)
{
	IntType<uint64_t> i;
	BOOST_CHECK_THROW(i = -1, value_error);
	i = 0;
	BOOST_CHECK_EQUAL(i.get(), 0);
	i = 0xffffffffffffffff;
	BOOST_CHECK_EQUAL(i.get(), 0xffffffffffffffff);
	// can't create constant with this value
	//BOOST_CHECK_THROW(i = 0x10000000000000000LLL, value_error);
}

BOOST_AUTO_TEST_CASE(range_check_int8_test)
{
	// range check error
	IntType<int8_t> i;
	BOOST_CHECK_THROW(i = -0x81, value_error);
	i = -0x80;
	BOOST_CHECK_EQUAL(i.get(), -0x80);
	i = 0x7f;
	BOOST_CHECK_EQUAL(i.get(), 0x7f);
	BOOST_CHECK_THROW(i = 0x80, value_error);
}

BOOST_AUTO_TEST_CASE(range_check_int16_test)
{
	IntType<int16_t> i;
	BOOST_CHECK_THROW(i = -0x8001, value_error);
	i = -0x8000;
	BOOST_CHECK_EQUAL(i.get(), -0x8000);
	i = 0x7fff;
	BOOST_CHECK_EQUAL(i.get(), 0x7fff);
	BOOST_CHECK_THROW(i = 0x8000, value_error);
}

BOOST_AUTO_TEST_CASE(range_check_int32_test)
{
	IntType<int32_t> i;
	BOOST_CHECK_THROW(i = -0x80000001L, value_error);
	i = -0x80000000L;
	BOOST_CHECK_EQUAL(i.get(), -0x80000000L);
	i = 0x7fffffff;
	BOOST_CHECK_EQUAL(i.get(), 0x7fffffff);
	BOOST_CHECK_THROW(i = 0x80000000L, value_error);
}

BOOST_AUTO_TEST_CASE(range_check_int64_test)
{
	IntType<int64_t> i;
	// can't create constant with this value
	//BOOST_CHECK_THROW(i = -0x8000000000000001LLL, value_error);
	// apparently can't create constant with this value either with gcc 4.4
	//i = -0x8000000000000000LL;
	//BOOST_CHECK_EQUAL(i.get(), -0x8000000000000000LL);
	i = 0x7fffffffffffffff;
	BOOST_CHECK_EQUAL(i.get(), 0x7fffffffffffffff);
	BOOST_CHECK_THROW(i = 0x8000000000000000, value_error);
}

BOOST_AUTO_TEST_CASE(assign_by_object_test)
{
	IntType<uint8_t> i;

	// check assigning value via another IntType
	IntType<uint8_t> j;
	j.get() = 100;
	i = j;
	BOOST_CHECK_EQUAL(i.get(), 100);

	// cannot change type on assignment
	BOOST_CHECK_THROW(i = Object(1.0f), type_error);
	BOOST_CHECK_THROW(i = Object('x'), type_error);
	BOOST_CHECK_THROW(i = Object(std::string("Hello world!")), type_error);

	// check assigning value via object
	i = Object((uint8_t)101);
	BOOST_CHECK_EQUAL(i.get(), 101);
}

BOOST_AUTO_TEST_SUITE_END()
