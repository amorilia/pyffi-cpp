/*

Copyright (c) 2007-2010, Python File Format Interface
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

#include "pyffi/object_models/map.hpp"

using namespace pyffi;
using namespace pyffi::object_models;

BOOST_AUTO_TEST_SUITE(map_test_suite)

BOOST_AUTO_TEST_CASE(declare_test)
{
	BOOST_CHECK_NO_THROW(Map<int> intmap);
}

BOOST_AUTO_TEST_CASE(add_test)
{
	Map<int> intmap;

	// add arguments
	BOOST_CHECK_NO_THROW(intmap.add("arg1", 5));
	BOOST_CHECK_NO_THROW(intmap.add("arg2", 2));
	BOOST_CHECK_NO_THROW(intmap.add("arg3", 9));

	// cannot add something that was already added
	BOOST_CHECK_THROW(intmap.add("arg2", 3), value_error);

	// check that value has not changed after bad add
	BOOST_CHECK_EQUAL(intmap.get("arg2"), 2);
}

BOOST_AUTO_TEST_CASE(get_test)
{
	Map<int> intmap;

	// add arguments
	BOOST_CHECK_NO_THROW(intmap.add("arg1", 5));
	BOOST_CHECK_NO_THROW(intmap.add("arg2", 2));
	BOOST_CHECK_NO_THROW(intmap.add("arg3", 9));

	// check values
	BOOST_CHECK_EQUAL(intmap.get("arg1"), 5);
	BOOST_CHECK_EQUAL(intmap.get("arg2"), 2);
	BOOST_CHECK_EQUAL(intmap.get("arg3"), 9);

	// cannot get something that was not yet added
	BOOST_CHECK_THROW(intmap.get("arg4"), key_error);

	// check value change
	BOOST_CHECK_EQUAL(intmap.get("arg2"), 2);
	BOOST_CHECK_NO_THROW(intmap.get("arg2") = 99);
	BOOST_CHECK_EQUAL(intmap.get("arg2"), 99);
}

BOOST_AUTO_TEST_SUITE_END()
