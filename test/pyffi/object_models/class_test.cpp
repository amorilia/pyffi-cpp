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
	PClass class0;
	PClass class1;
	PClass class2;
	PClass class3;
	PClass class4;
	PClass class5;

	BOOST_CHECK_NO_THROW(class0 = Class::class_());
	BOOST_CHECK_NO_THROW(class1 = class0->class_("TestClass1"));
	BOOST_CHECK_NO_THROW(class2 = class0->class_("TestClass2", class1));
	BOOST_CHECK_NO_THROW(class3 = class2->class_("TestClass3"));
	BOOST_CHECK_NO_THROW(class4 = class2->class_("TestClass4", class3));
	BOOST_CHECK_NO_THROW(class5 = class2->class_("TestClass5", class1));

	// check that class cannot be added again
	BOOST_CHECK_THROW(class0->class_("TestClass1"), value_error);
	BOOST_CHECK_THROW(class0->class_("TestClass2"), value_error);
	BOOST_CHECK_THROW(class2->class_("TestClass3"), value_error);
	BOOST_CHECK_THROW(class2->class_("TestClass4"), value_error);
	BOOST_CHECK_THROW(class2->class_("TestClass5"), value_error);
}

BOOST_AUTO_TEST_CASE(def_test)
{
	PClass class0 = Class::class_();

	// define attributes of various types
	BOOST_CHECK_NO_THROW(class0->def("arg1", 5));
	BOOST_CHECK_NO_THROW(class0->def("arg2", 'y'));
	BOOST_CHECK_NO_THROW(class0->def("arg3", std::string("Hello world!")));

	// check that attributes cannot be added again
	BOOST_CHECK_THROW(class0->def("arg1", 0), value_error);
	BOOST_CHECK_THROW(class0->def("arg2", 0), value_error);
	BOOST_CHECK_THROW(class0->def("arg3", 0), value_error);
}

BOOST_AUTO_TEST_CASE(attr_test)
{
	PClass class0 = Class::class_();
	PClass class1 = class0->class_("TestClass1");
	PClass class2 = class0->class_("TestClass2", class1);
	class1->def("arg1", 5);
	class2->def("arg2", 'y');

	// check if we get back the right classes
	BOOST_CHECK_EQUAL(class0->attr<PClass>("TestClass1"), class1);
	BOOST_CHECK_EQUAL(class0->attr<PClass>("TestClass2"), class2);

	// check that we cannot get something that hasn't been added yet
	BOOST_CHECK_THROW(class0->attr<PClass>("TestClass3"), name_error);

	// check if we get back the right values
	BOOST_CHECK_EQUAL(class1->attr<int>("arg1"), 5);
	BOOST_CHECK_EQUAL(class2->attr<char>("arg2"), 'y');

	// check that we cannot get attributes that don't exist yet
	BOOST_CHECK_THROW(class1->attr<int>("arg3"), name_error);
	BOOST_CHECK_THROW(class2->attr<int>("arg3"), name_error);
}

BOOST_AUTO_TEST_CASE(issubclass_test)
{
	// we basically test the following declarations:
	// class TestClass1:
	// class TestClass2(TestClass1)
	//     class TestClass3(TestClass1)
	// class TestClass4(TestClass3)
	PClass class0 = Class::class_();
	PClass class1 = class0->class_("TestClass1");
	PClass class2 = class0->class_("TestClass2", class1);
	PClass class3 = class2->class_("TestClass3", class1);
	PClass class4 = class0->class_("TestClass4", class3);

	// check subclass relationship
	BOOST_CHECK_EQUAL(class0->issubclass(class1), false);
	BOOST_CHECK_EQUAL(class0->issubclass(class2), false);
	BOOST_CHECK_EQUAL(class0->issubclass(class3), false);
	BOOST_CHECK_EQUAL(class0->issubclass(class4), false);
	BOOST_CHECK_EQUAL(class1->issubclass(class0), false);
	BOOST_CHECK_EQUAL(class1->issubclass(class2), false);
	BOOST_CHECK_EQUAL(class1->issubclass(class3), false);
	BOOST_CHECK_EQUAL(class1->issubclass(class4), false);
	BOOST_CHECK_EQUAL(class2->issubclass(class0), false);
	BOOST_CHECK_EQUAL(class2->issubclass(class1), true);
	BOOST_CHECK_EQUAL(class2->issubclass(class3), false);
	BOOST_CHECK_EQUAL(class2->issubclass(class4), false);
	BOOST_CHECK_EQUAL(class3->issubclass(class0), false);
	BOOST_CHECK_EQUAL(class3->issubclass(class1), true);
	BOOST_CHECK_EQUAL(class3->issubclass(class2), false);
	BOOST_CHECK_EQUAL(class3->issubclass(class4), false);
	BOOST_CHECK_EQUAL(class4->issubclass(class0), false);
	BOOST_CHECK_EQUAL(class4->issubclass(class1), true);
	BOOST_CHECK_EQUAL(class4->issubclass(class2), false);
	BOOST_CHECK_EQUAL(class4->issubclass(class3), true);
}

BOOST_AUTO_TEST_SUITE_END()
