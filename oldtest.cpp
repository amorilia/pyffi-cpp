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

#include <iostream>

#include "pyffi/object_models/format.hpp"
#include "pyffi/object_models/struct.hpp"

using namespace pyffi;

void init_testformat(Format & testformat)
{
	// testformat is initialized here explicitly
	// this could be done from an xml file
	PClass ObjClass = testformat.add_struct("ObjClass");
	ObjClass->add_attr("a number", 5);
	ObjClass->add_attr("a string attr", std::string("hello world"));

	PClass ObjClass2 = testformat.add_struct("ObjClass2");
	ObjClass2->add_attr<double>("x", 0.0);
	ObjClass2->add_attr<float>("y", 0.0);
	ObjClass2->add_attr("z", Struct(testformat.get_struct("ObjClass")));
}

int main()
{
	// set up format description

	Format testformat;
	init_testformat(testformat);

	// now we create a few structures which have been defined in testformat

	Struct obj(testformat.get_struct("ObjClass"));
	Struct obj2(testformat.get_struct("ObjClass2"));

	// check default values
	std::cout << obj.get_attr<int>("a number") << std::endl;
	std::cout << obj.get_attr<std::string>("a string attr") << std::endl;

	// change the values
	obj.get_attr<int>("a number") = 3;
	obj.get_attr<std::string>("a string attr") = "hello again!";

	std::cout << obj.get_attr<int>("a number") << std::endl;
	std::cout << obj.get_attr<std::string>("a string attr") << std::endl;
	std::cout << obj2.get_attr<Struct>("z").get_attr<std::string>("a string attr") << std::endl;

	// non-existing class
	testformat.get_struct("ClassWhichDoesNotExist"); // exception

	// non-existing attribute
	obj.get_attr<int>("non existing attribute"); // exception

	// bad cast
	std::cout << obj.get_attr<float>("a number") << std::endl; // exception

	return 0;
}
