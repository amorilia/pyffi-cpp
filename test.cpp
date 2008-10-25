#include <iostream>
#include "cffi/objectmodels/Attribute.hpp"
#include "cffi/objectmodels/MetaStruct.hpp"
#include "cffi/objectmodels/Struct.hpp"

int main() {
  MetaStruct ObjClass;
  ObjClass.addAttribute("a number", 5);
  ObjClass.addAttribute("a string attr", std::string("hello world"));

  Struct obj(ObjClass);
  // default values
  std::cout << boost::any_cast<int>(obj["a number"]) << std::endl;
  std::cout << boost::any_cast<std::string>(obj["a string attr"]) << std::endl;

  // change the values
  obj["a number"] = 3;
  obj["a string attr"] = std::string("hello again!");

  std::cout << obj["a number"].get<int>() << std::endl;
  std::cout << obj["a string attr"].get<std::string>() << std::endl;

  // bad cast
  std::cout << obj["a number"].get<short>() << std::endl; // exception

  // try to change the type
  obj["a number"] = 4.4; // exception

  return 0;
}

