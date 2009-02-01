#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ObjectTest
#include <boost/test/unit_test.hpp>

#include "cffi/object_models/object.hpp"
#include "cffi/exceptions.hpp"

BOOST_AUTO_TEST_CASE(constructor_test)
{
  // test default constructor
  //BOOST_CHECK_THROW(Object(), type_error);

  // test constructor with various types
  Object obj_int(20513);
  BOOST_CHECK_EQUAL(obj_int.get<int>(), 20513);
  Object obj_char('x');
  BOOST_CHECK_EQUAL(obj_char.get<char>(), 'x');
  Object obj_str(std::string("Hello world!"));
  BOOST_CHECK_EQUAL(obj_str.get<std::string>(), std::string("Hello world!"));

  // test copy constructor
  Object obj_int_copy(obj_int);
  BOOST_CHECK_EQUAL(obj_int.get<int>(), obj_int_copy.get<int>());
  Object obj_char_copy(obj_char);
  BOOST_CHECK_EQUAL(obj_char.get<char>(), obj_char_copy.get<char>());
  Object obj_str_copy(obj_str);
  BOOST_CHECK_EQUAL(obj_str.get<std::string>(), obj_str_copy.get<std::string>());
}

BOOST_AUTO_TEST_CASE(get_test)
{
  // check that get returns assigned value
  Object obj_short(short(5));
  BOOST_CHECK_EQUAL(obj_short.get<short>(), 5);

  // check bad type casts
  BOOST_CHECK_THROW(obj_short.get<int>(), type_error);
  BOOST_CHECK_THROW(obj_short.get<float>(), type_error);
  BOOST_CHECK_THROW(obj_short.get<char>(), type_error);
  BOOST_CHECK_THROW(obj_short.get<std::string>(), type_error);

  // check assignment via get
  obj_short.get<short>() = 10;
  BOOST_CHECK_EQUAL(obj_short.get<short>(), 10);
}

BOOST_AUTO_TEST_CASE(assign_by_value_test)
{
  Object obj(50);

  // check assigning different value
  obj = 100;
  BOOST_CHECK_EQUAL(obj.get<int>(), 100);

  // cannot change type on assignment
  BOOST_CHECK_THROW(obj = 1.0f, type_error);
  BOOST_CHECK_THROW(obj = 'x', type_error);
  BOOST_CHECK_THROW(obj = std::string("Hello world!"), type_error);
}

BOOST_AUTO_TEST_CASE(assign_by_object_test)
{
  Object obj(50);

  // check assigning different value
  obj = Object(100);
  BOOST_CHECK_EQUAL(obj.get<int>(), 100);

  // cannot change type on assignment
  BOOST_CHECK_THROW(obj = Object(1.0f), type_error);
  BOOST_CHECK_THROW(obj = Object('x'), type_error);
  BOOST_CHECK_THROW(obj = Object(std::string("Hello world!")), type_error);

  obj = Object(Object(101));
  BOOST_CHECK_EQUAL(obj.get<int>(), 101);

  obj = 102;
  obj = Object(obj);
  BOOST_CHECK_EQUAL(Object(obj).get<int>(), 102);
}
