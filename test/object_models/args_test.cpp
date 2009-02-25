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

#include "pyffi/object_models/args.hpp"
#include "pyffi/exceptions.hpp"

using namespace pyffi;

BOOST_AUTO_TEST_SUITE(args_test_suite)

BOOST_AUTO_TEST_CASE(declare_test) {
	BOOST_CHECK_NO_THROW(Args args);
}

BOOST_AUTO_TEST_CASE(add_test) {
	Args args;

	// add arguments of various types
	BOOST_CHECK_NO_THROW(args.add<int>("arg1", 5));
	BOOST_CHECK_NO_THROW(args.add<char>("arg2", 'y'));
	BOOST_CHECK_NO_THROW(args.add<std::string>("arg3", "Hello world!"));

	BOOST_CHECK_EQUAL(args.get<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(args.get<char>("arg2"), 'y');
	BOOST_CHECK_EQUAL(args.get<std::string>("arg3"), std::string("Hello world!"));

	// check that argument cannot be added again
	BOOST_CHECK_THROW(args.add<int>("arg1", 10), value_error);
	BOOST_CHECK_THROW(args.add<int>("arg2", 10), value_error);
	BOOST_CHECK_THROW(args.add<int>("arg3", 10), value_error);
}

BOOST_AUTO_TEST_CASE(constructor_test) {
	// test default constructor
	BOOST_CHECK_NO_THROW(Args());

	// test copy constructor
	Args args;
	args.add<int>("arg1", 5);
	args.add<char>("arg2", 'y');
	args.add<std::string>("arg3", "Hello world!");
	Args args2(args);
	BOOST_CHECK_NO_THROW(args2.get<int>("arg1"));
	BOOST_CHECK_NO_THROW(args2.get<char>("arg2"));
	BOOST_CHECK_NO_THROW(args2.get<std::string>("arg3"));
	BOOST_CHECK_EQUAL(args2.get<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(args2.get<char>("arg2"), 'y');
	BOOST_CHECK_EQUAL(args2.get<std::string>("arg3"), std::string("Hello world!"));

	// test that copy constructor makes a deep copy
	args.get<int>("arg1") = 2;
	BOOST_CHECK_EQUAL(args.get<int>("arg1"), 2);
	BOOST_CHECK_EQUAL(args2.get<int>("arg1"), 5);
}

BOOST_AUTO_TEST_CASE(get_test) {
	// set up arguments
	Args args;
	BOOST_CHECK_NO_THROW(args.add<char>("char1", 's'));
	BOOST_CHECK_NO_THROW(args.add<char>("char2", 'f'));
	BOOST_CHECK_NO_THROW(args.add<bool>("ic", true));

	// check that invalid get really does not create a key
	BOOST_CHECK_THROW(args.get<int>("test"), key_error);
	BOOST_CHECK_THROW(args.get<int>("test"), key_error);

	// check that type casts work as expected
	BOOST_CHECK_THROW(args.get<int>("char1"), type_error);
	BOOST_CHECK_NO_THROW(args.get<char>("char1"));
	BOOST_CHECK_THROW(args.get<std::string>("char1"), type_error);
	BOOST_CHECK_THROW(args.get<bool>("char1"), type_error);

	BOOST_CHECK_THROW(args.get<int>("char2"), type_error);
	BOOST_CHECK_NO_THROW(args.get<char>("char2"));
	BOOST_CHECK_THROW(args.get<std::string>("char2"), type_error);
	BOOST_CHECK_THROW(args.get<bool>("char2"), type_error);

	BOOST_CHECK_THROW(args.get<int>("ic"), type_error);
	BOOST_CHECK_THROW(args.get<char>("ic"), type_error);
	BOOST_CHECK_THROW(args.get<std::string>("ic"), type_error);
	BOOST_CHECK_NO_THROW(args.get<bool>("ic"));

	// check assignment via get
	args.get<char>("char2") = 'a';
	BOOST_CHECK_EQUAL(args.get<char>("char1"), 's');
	BOOST_CHECK_EQUAL(args.get<char>("char2"), 'a');
	BOOST_CHECK_EQUAL(args.get<bool>("ic"), true);

	args.get<char>("char1") = 'a';
	args.get<bool>("ic") = false;
	BOOST_CHECK_EQUAL(args.get<char>("char1"), 'a');
	BOOST_CHECK_EQUAL(args.get<char>("char2"), 'a');
	BOOST_CHECK_EQUAL(args.get<bool>("ic"), false);
}

BOOST_AUTO_TEST_SUITE_END()
