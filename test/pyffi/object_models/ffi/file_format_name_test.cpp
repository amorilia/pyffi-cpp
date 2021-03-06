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

#include "pyffi/object_models/ffi/file_format.hpp"

using namespace pyffi::object_models::ffi;

BOOST_AUTO_TEST_SUITE(file_format_name_test_suite)

BOOST_AUTO_TEST_CASE(name_test)
{
	FileFormat ff("test_name.ffi");
	/*
	std::vector<std::string> ext(ff.class_->attr<std::vector<std::string> >("extensions"));
	BOOST_CHECK_EQUAL(ext.size(), 1);
	BOOST_CHECK_EQUAL(ext.front(), "AFORMATNAMETEST123");
	*/
}

BOOST_AUTO_TEST_CASE(multi_name_test)
{
	FileFormat ff("test_multi_name.ffi");
	/*
	std::vector<std::string> ext = ff.class_->attr<std::vector<std::string> >("extensions");
	BOOST_CHECK_EQUAL(ext.size(), 6);
	BOOST_CHECK_EQUAL(ext[0], "AFORMATNAMETEST123");
	BOOST_CHECK_EQUAL(ext[1], "AND");
	BOOST_CHECK_EQUAL(ext[2], "A");
	BOOST_CHECK_EQUAL(ext[3], "FEW");
	BOOST_CHECK_EQUAL(ext[4], "OTHER");
	BOOST_CHECK_EQUAL(ext[5], "ONES");
	*/
}

BOOST_AUTO_TEST_SUITE_END()
