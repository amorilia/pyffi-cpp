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

BOOST_AUTO_TEST_SUITE(class_test_suite)

BOOST_AUTO_TEST_CASE(create_test)
{
	// we basically test the following declarations:
	// class TestClass1
	// class TestClass2(TestClass1):
	//     class TestClass3
	//     class TestClass4(TestClass3)
	//     class TestClass5(TestClass1)
	PClass ms0;
	PClass ms1;
	PClass ms2;
	PClass ms3;
	PClass ms4;
	PClass ms5;

	BOOST_CHECK_NO_THROW(ms0 = Class::scope());
	BOOST_CHECK_NO_THROW(ms1 = ms0->class_("TestClass1"));
	BOOST_CHECK_NO_THROW(ms2 = ms0->class_("TestClass2", ms1));
	BOOST_CHECK_NO_THROW(ms3 = ms2->class_("TestClass3"));
	BOOST_CHECK_NO_THROW(ms4 = ms2->class_("TestClass4", ms3));
	BOOST_CHECK_NO_THROW(ms5 = ms2->class_("TestClass5", ms1));

	// check that class cannot be added again
	BOOST_CHECK_THROW(ms0->class_("TestClass1"), value_error);
	BOOST_CHECK_THROW(ms0->class_("TestClass2"), value_error);
	BOOST_CHECK_THROW(ms2->class_("TestClass3"), value_error);
	BOOST_CHECK_THROW(ms2->class_("TestClass4"), value_error);
	BOOST_CHECK_THROW(ms2->class_("TestClass5"), value_error);
}

BOOST_AUTO_TEST_CASE(def_test)
{
	PClass ms = Class::scope();

	// define attributes of various types
	BOOST_CHECK_NO_THROW(ms->def("arg1", 5));
	BOOST_CHECK_NO_THROW(ms->def("arg2", 'y'));
	BOOST_CHECK_NO_THROW(ms->def("arg3", std::string("Hello world!")));

	// check that attributes cannot be added again
	BOOST_CHECK_THROW(ms->def("arg1", 0), value_error);
	BOOST_CHECK_THROW(ms->def("arg2", 0), value_error);
	BOOST_CHECK_THROW(ms->def("arg3", 0), value_error);
}

BOOST_AUTO_TEST_CASE(attr_test)
{
	PClass ms0 = Class::scope();
	PClass ms1 = ms0->class_("TestClass1");
	PClass ms2 = ms0->class_("TestClass2", ms1);
	ms1->def("arg1", 5);
	ms2->def("arg2", 'y');

	// check if we get back the right classes
	BOOST_CHECK_EQUAL(ms0->attr("TestClass1"), ms1);
	BOOST_CHECK_EQUAL(ms0->attr("TestClass2"), ms2);

	// check that we cannot get something that hasn't been added yet
	BOOST_CHECK_THROW(ms0->attr("TestClass3"), name_error);

	// check if we get back the right values
	BOOST_CHECK_EQUAL(ms1->attr<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(ms2->attr<char>("arg2"), 'y');

	// check that we cannot get attributes that don't exist yet
	BOOST_CHECK_THROW(ms1->attr<int>("arg3"), name_error);
	BOOST_CHECK_THROW(ms2->attr<int>("arg3"), name_error);
}

BOOST_AUTO_TEST_CASE(issubclass_test)
{
	// we basically test the following declarations:
	// class TestClass1:
	// class TestClass2(TestClass1)
	//     class TestClass3(TestClass1)
	// class TestClass4(TestClass3)
	PClass ms0 = Class::scope();
	PClass ms1 = ms0->class_("TestClass1");
	PClass ms2 = ms0->class_("TestClass2", ms1);
	PClass ms3 = ms2->class_("TestClass3", ms1);
	PClass ms4 = ms0->class_("TestClass4", ms3);

	// check subclass relationship
	BOOST_CHECK_EQUAL(ms0->issubclass(ms1), false);
	BOOST_CHECK_EQUAL(ms0->issubclass(ms2), false);
	BOOST_CHECK_EQUAL(ms0->issubclass(ms3), false);
	BOOST_CHECK_EQUAL(ms0->issubclass(ms4), false);
	BOOST_CHECK_EQUAL(ms1->issubclass(ms0), false);
	BOOST_CHECK_EQUAL(ms1->issubclass(ms2), false);
	BOOST_CHECK_EQUAL(ms1->issubclass(ms3), false);
	BOOST_CHECK_EQUAL(ms1->issubclass(ms4), false);
	BOOST_CHECK_EQUAL(ms2->issubclass(ms0), false);
	BOOST_CHECK_EQUAL(ms2->issubclass(ms1), true);
	BOOST_CHECK_EQUAL(ms2->issubclass(ms3), false);
	BOOST_CHECK_EQUAL(ms2->issubclass(ms4), false);
	BOOST_CHECK_EQUAL(ms3->issubclass(ms0), false);
	BOOST_CHECK_EQUAL(ms3->issubclass(ms1), true);
	BOOST_CHECK_EQUAL(ms3->issubclass(ms2), false);
	BOOST_CHECK_EQUAL(ms3->issubclass(ms4), false);
	BOOST_CHECK_EQUAL(ms4->issubclass(ms0), false);
	BOOST_CHECK_EQUAL(ms4->issubclass(ms1), true);
	BOOST_CHECK_EQUAL(ms4->issubclass(ms2), false);
	BOOST_CHECK_EQUAL(ms4->issubclass(ms3), true);
}

BOOST_AUTO_TEST_SUITE_END()
