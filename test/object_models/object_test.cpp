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
#define BOOST_TEST_MODULE ObjectTest
#include <boost/test/unit_test.hpp>

#include "pyffi/object_models/object.hpp"
#include "pyffi/exceptions.hpp"

using namespace pyffi;

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

BOOST_AUTO_TEST_CASE(shared_object_test)
{
  PObject p_obj1(new Object(10));
  PObject p_obj2(p_obj1);

  // check that the value is the same
  BOOST_CHECK_EQUAL(p_obj2->get<int>(), 10);

  // check that value is shared
  *p_obj1 = 20;
  BOOST_CHECK_EQUAL(p_obj2->get<int>(), 20);
}
