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

#include "pyffi/object_models/class.hpp"
#include "pyffi/exceptions.hpp"

using namespace pyffi;
using namespace pyffi::object_models;

BOOST_AUTO_TEST_SUITE(instance_test_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
	PClass scope = Class::scope();

	// add arguments of various types
	BOOST_CHECK_NO_THROW(scope->def("arg1", 5));
	BOOST_CHECK_NO_THROW(scope->def("arg2", 'y'));
	BOOST_CHECK_NO_THROW(scope->def("arg3", std::string("Hello world!")));

	// instantiate
	BOOST_CHECK_NO_THROW(scope());
}

BOOST_AUTO_TEST_CASE(attr_test)
{
	PClass scope = Class::scope();

	// add attributes of various types
	BOOST_CHECK_NO_THROW(scope->def("arg1", 5));
	BOOST_CHECK_NO_THROW(scope->def("arg2", 'y'));
	BOOST_CHECK_NO_THROW(scope->def("arg3", std::string("Hello world!")));

	// instantiate
	PInstance i1 = scope();
	PInstance i2 = scope();

	// check attributes
	BOOST_CHECK_EQUAL(i1->attr<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(i1->attr<char>("arg2"), 'y');
	BOOST_CHECK_EQUAL(i1->attr<std::string>("arg3"), "Hello world!");
	BOOST_CHECK_EQUAL(i2->attr<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(i2->attr<char>("arg2"), 'y');
	BOOST_CHECK_EQUAL(i2->attr<std::string>("arg3"), "Hello world!");

	// change some instance attributes
	i1->attr<int>("arg1") = 3;
	i1->attr<char>("arg2") = 'b';
	i1->attr<std::string>("arg3") = "yay";
	i2->attr<int>("arg1") = 9;
	i2->attr<char>("arg2") = 'g';
	i2->attr<std::string>("arg3") = "doh";

	// check that the attributes have actually changed
	BOOST_CHECK_EQUAL(i1->attr<int>("arg1"), 3);
	BOOST_CHECK_EQUAL(i1->attr<char>("arg2"), 'b');
	BOOST_CHECK_EQUAL(i1->attr<std::string>("arg3"), "yay");
	BOOST_CHECK_EQUAL(i2->attr<int>("arg1"), 9);
	BOOST_CHECK_EQUAL(i2->attr<char>("arg2"), 'g');
	BOOST_CHECK_EQUAL(i2->attr<std::string>("arg3"), "doh");

	// check that class attributes are still the same
	BOOST_CHECK_EQUAL(scope->attr<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(scope->attr<char>("arg2"), 'y');
	BOOST_CHECK_EQUAL(scope->attr<std::string>("arg3"), "Hello world!");

	// change some class attributes
	scope->attr<int>("arg1") = 100;
	scope->attr<char>("arg2") = 'z';
	scope->attr<std::string>("arg3") = "yes";

	// check that the attributes have not changed
	BOOST_CHECK_EQUAL(i1->attr<int>("arg1"), 3);
	BOOST_CHECK_EQUAL(i1->attr<char>("arg2"), 'b');
	BOOST_CHECK_EQUAL(i1->attr<std::string>("arg3"), "yay");
	BOOST_CHECK_EQUAL(i2->attr<int>("arg1"), 9);
	BOOST_CHECK_EQUAL(i2->attr<char>("arg2"), 'g');
	BOOST_CHECK_EQUAL(i2->attr<std::string>("arg3"), "doh");

	// instantiate and check attributes
	PInstance i3 = scope();
	BOOST_CHECK_EQUAL(i3->attr<int>("arg1"), 100);
	BOOST_CHECK_EQUAL(i3->attr<char>("arg2"), 'z');
	BOOST_CHECK_EQUAL(i3->attr<std::string>("arg3"), "yes");
}

BOOST_AUTO_TEST_SUITE_END()
