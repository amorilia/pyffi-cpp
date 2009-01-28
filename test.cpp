#include <iostream>
#include <boost/shared_ptr.hpp>

#include "cffi/objectmodels/Attribute.hpp"
#include "cffi/objectmodels/MetaStruct.hpp"
#include "cffi/objectmodels/Struct.hpp"
#include "cffi/objectmodels/Format.hpp"

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
  std::cout << boost::any_cast<int>(obj["a number"]) << std::endl;
  std::cout << boost::any_cast<std::string>(obj["a string attr"]) << std::endl;

  // change the values
  obj["a number"] = 3;
  obj["a string attr"] = std::string("hello again!");

  std::cout << obj["a number"].get<int>() << std::endl;
  std::cout << obj["a string attr"].get<std::string>() << std::endl;
  std::cout << obj2["z"].get<Struct>()["a string attr"].get<std::string>() << std::endl;

  // non-existing class
  testformat["ClassWhichDoesNotExist"];

  // bad cast
  std::cout << obj["a number"].get<short>() << std::endl; // exception

  // try to change the type
  obj["a number"] = 4.4; // exception

  return 0;
}

