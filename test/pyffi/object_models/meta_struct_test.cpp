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

#include "pyffi/object_models/meta_struct.hpp"
#include "pyffi/exceptions.hpp"

using namespace pyffi;
using namespace pyffi::object_models;

BOOST_AUTO_TEST_SUITE(meta_struct_test_suite)

BOOST_AUTO_TEST_CASE(declare_test) {
	BOOST_CHECK_NO_THROW(MetaStruct ms);
}

BOOST_AUTO_TEST_CASE(add_test) {
	MetaStruct ms;
	PMetaAttribute ma1(new MetaAttribute(5));
	PMetaAttribute ma2(new MetaAttribute('y'));
	PMetaAttribute ma3(new MetaAttribute(std::string("Hello world!")));

	// add arguments of various types
	BOOST_CHECK_NO_THROW(ms.add("arg1", ma1));
	BOOST_CHECK_NO_THROW(ms.add("arg2", ma2));
	BOOST_CHECK_NO_THROW(ms.add("arg3", ma3));

	// check that argument cannot be added again
	PMetaAttribute ma4(new MetaAttribute(999));
	BOOST_CHECK_THROW(ms.add("arg1", ma4), value_error);
	BOOST_CHECK_THROW(ms.add("arg2", ma4), value_error);
	BOOST_CHECK_THROW(ms.add("arg3", ma4), value_error);
}

BOOST_AUTO_TEST_CASE(index_test) {
	MetaStruct ms;
	PMetaAttribute ma1(new MetaAttribute(5));
	PMetaAttribute ma2(new MetaAttribute('y'));
	PMetaAttribute ma3(new MetaAttribute(std::string("Hello world!")));

	// add arguments of various types
	BOOST_CHECK_NO_THROW(ms.add("arg1", ma1));
	BOOST_CHECK_NO_THROW(ms.add("arg2", ma2));
	BOOST_CHECK_NO_THROW(ms.add("arg3", ma3));

	// check that index matches number of argument
	BOOST_CHECK_EQUAL(ms.index("arg1"), 0);
	BOOST_CHECK_EQUAL(ms.index("arg2"), 1);
	BOOST_CHECK_EQUAL(ms.index("arg3"), 2);
}

BOOST_AUTO_TEST_SUITE_END()
