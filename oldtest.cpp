#include <iostream>

#include "pyffi/object_models/format.hpp"
#include "pyffi/object_models/struct.hpp"

using namespace pyffi;

void init_testformat(Format & testformat) {
	// testformat is initialized here explicitly
	// this could be done from an xml file
	PMetaStruct ObjClass = testformat.add_struct("ObjClass");
	ObjClass->add_attr("a number", 5);
	ObjClass->add_attr("a string attr", std::string("hello world"));

	PMetaStruct ObjClass2 = testformat.add_struct("ObjClass2");
	ObjClass2->add_attr<double>("x", 0.0);
	ObjClass2->add_attr<float>("y", 0.0);
	ObjClass2->add_attr("z", Struct(testformat.get_struct("ObjClass")));
}

int main() {
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
