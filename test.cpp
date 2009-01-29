#include <iostream>
#include <boost/shared_ptr.hpp>

#include "cffi/object_models/format.hpp"
#include "cffi/object_models/struct.hpp"

void init_testformat(Format & testformat) {
  // testformat is initialized here explicitly
  // this could be done from an xml file
  boost::shared_ptr<MetaStruct> ObjClass = testformat.new_struct("ObjClass");
  ObjClass->add_attribute("a number", 5);
  ObjClass->add_attribute("a string attr", std::string("hello world"));

  boost::shared_ptr<MetaStruct> ObjClass2 = testformat.new_struct("ObjClass2");
  ObjClass2->add_attribute("x", 0.0);
  ObjClass2->add_attribute("y", 0.0);
  Struct some_default(testformat["ObjClass"]);
  ObjClass2->add_attribute("z", some_default);
}

int main() {
  // set up format description

  Format testformat;
  init_testformat(testformat);

  // now we create a few structures which have been defined in testformat

  Struct obj(testformat["ObjClass"]);
  Struct obj2(testformat["ObjClass2"]);

  // check default values
  std::cout << obj.get<int>("a number") << std::endl;
  std::cout << obj.get<std::string>("a string attr") << std::endl;

  // change the values
  obj.get<int>("a number") = 3;
  obj.get<std::string>("a string attr") = "hello again!";

  std::cout << obj.get<int>("a number") << std::endl;
  std::cout << obj.get<std::string>("a string attr") << std::endl;
  std::cout << obj2.get<Struct>("z").get<std::string>("a string attr") << std::endl;

  // non-existing class
  //testformat["ClassWhichDoesNotExist"];

  // non-existing attribute
  obj.get<int>("non existing attribute");

  // bad cast
  std::cout << obj.get<short>("a number") << std::endl; // exception

  // try to change the type
  obj.get<float>("a number") = 4.4; // exception

  return 0;
}
