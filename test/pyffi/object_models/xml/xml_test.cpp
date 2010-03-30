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

#include "pyffi/exceptions.hpp"
#include "pyffi/object_models/class.hpp"
#include "pyffi/object_models/xml/file_format.hpp"

typedef pyffi::object_models::PClass PClass;
typedef pyffi::object_models::xml::FileFormat FileFormat;

BOOST_AUTO_TEST_SUITE(xml_test_suite)

BOOST_AUTO_TEST_CASE(non_existing_file_test)
{
	BOOST_CHECK_THROW(FileFormat("non_existing_file.xml"), pyffi::io_error);
}

BOOST_AUTO_TEST_CASE(invalid_test)
{
	BOOST_CHECK_THROW(FileFormat("test_invalid.xml"), pyffi::syntax_error);
}

BOOST_AUTO_TEST_CASE(minimal_test)
{
	FileFormat("test_minimal.xml");
}

BOOST_AUTO_TEST_CASE(header_test)
{
	FileFormat("test_header.xml");
}

BOOST_AUTO_TEST_CASE(version_test)
{
	FileFormat("test_version.xml");
}

BOOST_AUTO_TEST_CASE(basic_test)
{
	FileFormat("test_basic.xml");
}

BOOST_AUTO_TEST_CASE(enum_test)
{
	FileFormat("test_enum.xml");
}

BOOST_AUTO_TEST_CASE(full_test)
{
	FileFormat ff("test_full.xml");
	PClass NiObject = ff.class_->attr<PClass>("NiObject");
	PClass NiExtraData = ff.class_->attr<PClass>("NiExtraData");
	PClass NiObjectNET = ff.class_->attr<PClass>("NiObjectNET");
	PClass NiAVObject = ff.class_->attr<PClass>("NiAVObject");
	BOOST_CHECK_EQUAL(NiExtraData->issubclass(NiObject), true);
	BOOST_CHECK_EQUAL(NiAVObject->issubclass(NiObject), true);
	BOOST_CHECK_EQUAL(NiExtraData->issubclass(NiAVObject), false);
}

BOOST_AUTO_TEST_SUITE_END()
