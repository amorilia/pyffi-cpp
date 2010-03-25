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

#include "pyffi/object_models/object_map.hpp"
#include "pyffi/exceptions.hpp"

using namespace pyffi;
using namespace pyffi::object_models;

BOOST_AUTO_TEST_SUITE(object_map_test_suite)

BOOST_AUTO_TEST_CASE(declare_test)
{
	BOOST_CHECK_NO_THROW(ObjectMap omap);
}

BOOST_AUTO_TEST_CASE(add_test)
{
	ObjectMap omap;

	// add arguments of various types
	BOOST_CHECK_NO_THROW(omap.add<int>("arg1", 5));
	BOOST_CHECK_NO_THROW(omap.add<char>("arg2", 'y'));
	BOOST_CHECK_NO_THROW(omap.add<std::string>("arg3", "Hello world!"));

	BOOST_CHECK_EQUAL(omap.get<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(omap.get<char>("arg2"), 'y');
	BOOST_CHECK_EQUAL(omap.get<std::string>("arg3"), std::string("Hello world!"));

	// check that argument cannot be added again
	BOOST_CHECK_THROW(omap.add<int>("arg1", 10), value_error);
	BOOST_CHECK_THROW(omap.add<int>("arg2", 10), value_error);
	BOOST_CHECK_THROW(omap.add<int>("arg3", 10), value_error);
}

BOOST_AUTO_TEST_CASE(constructor_test)
{
	// test default constructor
	BOOST_CHECK_NO_THROW(ObjectMap());

	// test copy constructor
	ObjectMap omap;
	omap.add<int>("arg1", 5);
	omap.add<char>("arg2", 'y');
	omap.add<std::string>("arg3", "Hello world!");
	ObjectMap omap2(omap);
	BOOST_CHECK_NO_THROW(omap2.get<int>("arg1"));
	BOOST_CHECK_NO_THROW(omap2.get<char>("arg2"));
	BOOST_CHECK_NO_THROW(omap2.get<std::string>("arg3"));
	BOOST_CHECK_EQUAL(omap2.get<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(omap2.get<char>("arg2"), 'y');
	BOOST_CHECK_EQUAL(omap2.get<std::string>("arg3"), std::string("Hello world!"));

	// test that copy constructor makes a deep copy
	omap.get<int>("arg1") = 2;
	BOOST_CHECK_EQUAL(omap.get<int>("arg1"), 2);
	BOOST_CHECK_EQUAL(omap2.get<int>("arg1"), 5);
}

BOOST_AUTO_TEST_CASE(get_test)
{
	// set up arguments
	ObjectMap omap;
	BOOST_CHECK_NO_THROW(omap.add<char>("char1", 's'));
	BOOST_CHECK_NO_THROW(omap.add<char>("char2", 'f'));
	BOOST_CHECK_NO_THROW(omap.add<bool>("ic", true));

	// check that invalid get really does not create a key
	BOOST_CHECK_THROW(omap.get<int>("test"), key_error);
	BOOST_CHECK_THROW(omap.get<int>("test"), key_error);

	// check that type casts work as expected
	BOOST_CHECK_THROW(omap.get<int>("char1"), type_error);
	BOOST_CHECK_NO_THROW(omap.get<char>("char1"));
	BOOST_CHECK_THROW(omap.get<std::string>("char1"), type_error);
	BOOST_CHECK_THROW(omap.get<bool>("char1"), type_error);

	BOOST_CHECK_THROW(omap.get<int>("char2"), type_error);
	BOOST_CHECK_NO_THROW(omap.get<char>("char2"));
	BOOST_CHECK_THROW(omap.get<std::string>("char2"), type_error);
	BOOST_CHECK_THROW(omap.get<bool>("char2"), type_error);

	BOOST_CHECK_THROW(omap.get<int>("ic"), type_error);
	BOOST_CHECK_THROW(omap.get<char>("ic"), type_error);
	BOOST_CHECK_THROW(omap.get<std::string>("ic"), type_error);
	BOOST_CHECK_NO_THROW(omap.get<bool>("ic"));

	// check assignment via get
	omap.get<char>("char2") = 'a';
	BOOST_CHECK_EQUAL(omap.get<char>("char1"), 's');
	BOOST_CHECK_EQUAL(omap.get<char>("char2"), 'a');
	BOOST_CHECK_EQUAL(omap.get<bool>("ic"), true);

	omap.get<char>("char1") = 'a';
	omap.get<bool>("ic") = false;
	BOOST_CHECK_EQUAL(omap.get<char>("char1"), 'a');
	BOOST_CHECK_EQUAL(omap.get<char>("char2"), 'a');
	BOOST_CHECK_EQUAL(omap.get<bool>("ic"), false);
}

BOOST_AUTO_TEST_SUITE_END()
