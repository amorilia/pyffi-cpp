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

BOOST_AUTO_TEST_CASE(constructor_test) {
	// test default constructor
	IntType<uint8_t> i;
	BOOST_CHECK_EQUAL(i.get(), 0);

	// test copy constructor
	i = 10;
	IntType<uint8_t> j(i);
	BOOST_CHECK_EQUAL(i.get(), j.get());
}

BOOST_AUTO_TEST_CASE(get_test) {
	// check that get returns assigned value
	IntType<uint8_t> i;
	i = 5;
	BOOST_CHECK_EQUAL(i.get(), 5);

	// check changing assignment via get
	i.get() = 6;
	BOOST_CHECK_EQUAL(i.get(), 6);
}

BOOST_AUTO_TEST_CASE(assign_by_value_test) {
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

	// range check error
	BOOST_CHECK_THROW(i = 256, value_error);
	BOOST_CHECK_THROW(i = -1, value_error);
}

BOOST_AUTO_TEST_CASE(assign_by_object_test) {
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
