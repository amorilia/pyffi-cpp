#include <iostream>
#include <boost/shared_ptr.hpp>

#include "cffi/objectmodels/Attribute.hpp"
#include "cffi/objectmodels/MetaStruct.hpp"
#include "cffi/objectmodels/Struct.hpp"

int main() {
  boost::shared_ptr<MetaStruct> ObjClass(new MetaStruct);
  ObjClass->add_attribute("a number", 5);
  ObjClass->add_attribute("a string attr", std::string("hello world"));

  Struct obj(ObjClass);

  boost::shared_ptr<MetaStruct> ObjClass2(new MetaStruct);
  ObjClass2->add_attribute("x", 0.0);
  ObjClass2->add_attribute("y", 0.0);
  ObjClass2->add_attribute("z", obj);

  Struct obj2(ObjClass2);

  // default values
  std::cout << boost::any_cast<int>(obj["a number"]) << std::endl;
  std::cout << boost::any_cast<std::string>(obj["a string attr"]) << std::endl;

  // change the values
  obj["a number"] = 3;
  obj["a string attr"] = std::string("hello again!");

  std::cout << obj["a number"].get<int>() << std::endl;
  std::cout << obj["a string attr"].get<std::string>() << std::endl;
  std::cout << obj2["z"].get<Struct>()["a string attr"].get<std::string>() << std::endl;

  // bad cast
  std::cout << obj["a number"].get<short>() << std::endl; // exception

  // try to change the type
  obj["a number"] = 4.4; // exception

  return 0;
}

