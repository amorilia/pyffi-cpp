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

using namespace boost;
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
	PClass class6;
	PClass class7;

	BOOST_CHECK_NO_THROW(class0 = Class::class_());
	// create classes
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
	PClass Int = class0->class_("Int");
	Int->primitive(5);
	PClass Char = class0->class_("Char");
	Char->primitive('y');
	PClass String = class0->class_("String");
	String->primitive("Hello world!");

	// define attributes of various types
	BOOST_CHECK_NO_THROW(class0->def("arg1", Int));
	BOOST_CHECK_NO_THROW(class0->def("arg2", Char));
	BOOST_CHECK_NO_THROW(class0->def("arg3", String));

	// check that attributes cannot be added again
	BOOST_CHECK_THROW(class0->def("arg1", Int), value_error);
	BOOST_CHECK_THROW(class0->def("arg2", Char), value_error);
	BOOST_CHECK_THROW(class0->def("arg3", String), value_error);
}

BOOST_AUTO_TEST_CASE(get_def_class_test)
{
	PClass class0 = Class::class_();
	PClass Int = class0->class_("Int");
	Int->primitive(5);
	PClass Char = class0->class_("Char");
	Char->primitive('y');
	PClass String = class0->class_("String");
	String->primitive("Hello world!");
	class0->def("arg1", Int);
	class0->def("arg2", Char);
	class0->def("arg3", String);

	BOOST_CHECK_EQUAL(class0->get_def_class("arg1"), Int);
	BOOST_CHECK_EQUAL(class0->get_def_class("arg2"), Char);
	BOOST_CHECK_EQUAL(class0->get_def_class("arg3"), String);

	BOOST_CHECK_THROW(class0->get_def_class("arg99"), name_error);
}

BOOST_AUTO_TEST_CASE(get_class_test)
{
	PClass class0 = Class::class_();
	PClass Int = class0->class_("Int");
	Int->primitive(5);
	PClass Char = class0->class_("Char");
	Char->primitive('y');
	PClass class1 = class0->class_("TestClass1");
	class1->def("arg1", Int);
	class1->def("arg2", Int);
	PClass class2 = class0->class_("TestClass2", class1);
	class2->def("arg3", Char);
	class2->def("arg4", Int);

	// check if we get back the right classes
	BOOST_CHECK_EQUAL(class0->get_class("TestClass1"), class1);
	BOOST_CHECK_EQUAL(class0->get_class("TestClass2"), class2);

	// check that we don't get an argument through get_class
	BOOST_CHECK_THROW(class1->get_class("arg1"), name_error);
	BOOST_CHECK_THROW(class1->get_class("arg2"), name_error);
	BOOST_CHECK_THROW(class2->get_class("arg3"), name_error);
	BOOST_CHECK_THROW(class2->get_class("arg4"), name_error);

	// check that we cannot get something that hasn't been added yet
	BOOST_CHECK_THROW(class0->get_class("TestClass3"), name_error);
}

BOOST_AUTO_TEST_CASE(get_def_index_test)
{
	PClass class0 = Class::class_();
	PClass Int = class0->class_("Int");
	Int->primitive(5);
	PClass Char = class0->class_("Char");
	Char->primitive('y');
	PClass class1 = class0->class_("TestClass1");
	class1->def("arg1", Int);
	class1->def("arg2", Int);
	PClass class2 = class0->class_("TestClass2", class1);
	class2->def("arg3", Char);
	class2->def("arg4", Int);

	// check if we get back the right indices
	BOOST_CHECK_EQUAL(class1->get_def_index("arg1"), 0);
	BOOST_CHECK_EQUAL(class1->get_def_index("arg2"), 1);
	BOOST_CHECK_EQUAL(class2->get_def_index("arg3"), 2);
	BOOST_CHECK_EQUAL(class2->get_def_index("arg4"), 3);

	// check that we cannot get attributes that don't exist
	BOOST_CHECK_THROW(class1->get_def_index("arg3"), name_error);
	BOOST_CHECK_THROW(class1->get_def_index("arg4"), name_error);
	BOOST_CHECK_THROW(class1->get_def_index("arg99"), name_error);
	BOOST_CHECK_THROW(class2->get_def_index("arg99"), name_error);
}

BOOST_AUTO_TEST_CASE(is_subclass_test)
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
	BOOST_CHECK_EQUAL(class0->is_subclass(class1), false);
	BOOST_CHECK_EQUAL(class0->is_subclass(class2), false);
	BOOST_CHECK_EQUAL(class0->is_subclass(class3), false);
	BOOST_CHECK_EQUAL(class0->is_subclass(class4), false);
	BOOST_CHECK_EQUAL(class1->is_subclass(class0), false);
	BOOST_CHECK_EQUAL(class1->is_subclass(class2), false);
	BOOST_CHECK_EQUAL(class1->is_subclass(class3), false);
	BOOST_CHECK_EQUAL(class1->is_subclass(class4), false);
	BOOST_CHECK_EQUAL(class2->is_subclass(class0), false);
	BOOST_CHECK_EQUAL(class2->is_subclass(class1), true);
	BOOST_CHECK_EQUAL(class2->is_subclass(class3), false);
	BOOST_CHECK_EQUAL(class2->is_subclass(class4), false);
	BOOST_CHECK_EQUAL(class3->is_subclass(class0), false);
	BOOST_CHECK_EQUAL(class3->is_subclass(class1), true);
	BOOST_CHECK_EQUAL(class3->is_subclass(class2), false);
	BOOST_CHECK_EQUAL(class3->is_subclass(class4), false);
	BOOST_CHECK_EQUAL(class4->is_subclass(class0), false);
	BOOST_CHECK_EQUAL(class4->is_subclass(class1), true);
	BOOST_CHECK_EQUAL(class4->is_subclass(class2), false);
	BOOST_CHECK_EQUAL(class4->is_subclass(class3), true);
}

BOOST_AUTO_TEST_CASE(get_class_nested_test)
{
	// we basically test the following declarations:
	// class TestClass1
	// class TestClass2(TestClass1):
	//     class TestClass3
	//     class TestClass4(TestClass3)
	//     class TestClass5(TestClass1)
	PClass class0 = Class::class_();
	PClass class1 = class0->class_("TestClass1");
	PClass class2 = class0->class_("TestClass2", class1);
	PClass class3 = class2->class_("TestClass3");
	PClass class4 = class2->class_("TestClass4", class3);
	PClass class5 = class2->class_("TestClass5", class1);

	// check that all classes can be accessed from TestClass5
	BOOST_CHECK_EQUAL(class1, class5->get_class("TestClass1"));
	BOOST_CHECK_EQUAL(class2, class5->get_class("TestClass2"));
	BOOST_CHECK_EQUAL(class3, class5->get_class("TestClass3"));
	BOOST_CHECK_EQUAL(class4, class5->get_class("TestClass4"));
}

BOOST_AUTO_TEST_CASE(instance_test)
{
	PClass ns = Class::class_();
	PClass Int = ns->class_("Int");
	Int->primitive(5);
	PClass Char = ns->class_("Char");
	Char->primitive('y');
	PClass Class1 = ns->class_("TestClass1");
	Class1->def("arg1", Int);
	Class1->def("arg2", Int);
	PClass Class2 = ns->class_("TestClass2", Class1);
	Class2->def("arg3", Char);
	Class2->def("arg4", Int);

	// check that creating instances works
	BOOST_CHECK_NO_THROW(Int->instance());
	BOOST_CHECK_NO_THROW(Char->instance());
	BOOST_CHECK_NO_THROW(Class1->instance());
	BOOST_CHECK_NO_THROW(Class2->instance());

	// created some instances
	Object i1(Int->instance());
	Object i2(Char->instance());
	Object i3(Class1->instance());
	Object i4(Class2->instance());

	// check values of the instances
	BOOST_CHECK_EQUAL(get<int>(i1), 5);
	BOOST_CHECK_EQUAL(get<char>(i2), 'y');
	std::vector<Object> & i3vec = boost::get<std::vector<Object> >(i3);
	std::vector<Object> & i4vec = boost::get<std::vector<Object> >(i4);

	BOOST_CHECK_EQUAL(i3vec.size(), 2);
	BOOST_CHECK_EQUAL(get<int>(i3vec[0]), 5);
	BOOST_CHECK_EQUAL(get<int>(i3vec[1]), 5);
	BOOST_CHECK_EQUAL(i4vec.size(), 4);
	BOOST_CHECK_EQUAL(get<int>(i4vec[0]), 5);
	BOOST_CHECK_EQUAL(get<int>(i4vec[1]), 5);
	BOOST_CHECK_EQUAL(get<char>(i4vec[2]), 'y');
	BOOST_CHECK_EQUAL(get<int>(i4vec[3]), 5);

	// print the instances
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i1), "5");
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i2), "y");
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i3), "( 5 5 )");
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i4), "( 5 5 y 5 )");

	// change some values
	get<int>(i1) = 6;
	get<char>(i2) = 'z';
	get<int>(i3vec[0]) = 7;
	get<int>(i3vec[1]) = 8;
	get<int>(i4vec[0]) = 9;
	get<int>(i4vec[1]) = 12;
	get<char>(i4vec[2]) = 'c';
	get<int>(i4vec[3]) = 19;

	// check changed values
	BOOST_CHECK_EQUAL(get<int>(i1), 6);
	BOOST_CHECK_EQUAL(get<char>(i2), 'z');
	BOOST_CHECK_EQUAL(get<int>(i3vec[0]), 7);
	BOOST_CHECK_EQUAL(get<int>(i3vec[1]), 8);
	BOOST_CHECK_EQUAL(get<int>(i4vec[0]), 9);
	BOOST_CHECK_EQUAL(get<int>(i4vec[1]), 12);
	BOOST_CHECK_EQUAL(get<char>(i4vec[2]), 'c');
	BOOST_CHECK_EQUAL(get<int>(i4vec[3]), 19);

	// print the instances
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i1), "6");
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i2), "z");
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i3), "( 7 8 )");
	BOOST_CHECK_EQUAL(boost::apply_visitor(object_value_string(), i4), "( 9 12 c 19 )");
}

BOOST_AUTO_TEST_SUITE_END()
