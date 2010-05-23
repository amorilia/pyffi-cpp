/*

Copyright (c) 2007-2010, Python File Format Interface
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

#include "pyffi/object_models/expression.hpp"
#include "pyffi/exceptions.hpp"

using namespace pyffi;
using namespace pyffi::object_models;

BOOST_AUTO_TEST_SUITE(expression_test_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
	// construct a few expressions and check that they don't throw
	BOOST_CHECK_NO_THROW(Expression(1));
	BOOST_CHECK_NO_THROW(Expression("hello"));
	BOOST_CHECK_NO_THROW(Expression(3.2f));
}

BOOST_AUTO_TEST_CASE(expression_object_test)
{
	Object obj;
	// evaluate very simple atomic expressions
	Expression e1(1);
	Expression e2(2);
	obj = expression_object<int>(e1);
	BOOST_CHECK_EQUAL(boost::get<int>(obj), 1);
	obj = expression_object<int>(e2);
	BOOST_CHECK_EQUAL(boost::get<int>(obj), 2);
	// check that 1 + 2 = 3
	Expression e1plus2(binary_operator<op_plus>(e1, e2));
	obj = expression_object<int>(e1plus2);
	BOOST_CHECK_EQUAL(boost::get<int>(obj), 3);
}

BOOST_AUTO_TEST_CASE(expression_numeric_cast_test)
{
	int i;
	// evaluate very simple atomic expressions
	Expression e1(1);
	Expression e2(2);
	i = expression_numeric_cast<int, int>(e1);
	BOOST_CHECK_EQUAL(i, 1);
	i = expression_numeric_cast<int, int>(e2);
	BOOST_CHECK_EQUAL(i, 2);
	// check that 1 + 2 = 3
	Expression e1plus2(binary_operator<op_plus>(e1, e2));
	i = expression_numeric_cast<int, int>(e1plus2);
	BOOST_CHECK_EQUAL(i, 3);
}

BOOST_AUTO_TEST_SUITE_END()
