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

#ifndef PYFFI_OM_EXPRESSION_HPP_INCLUDED
#define PYFFI_OM_EXPRESSION_HPP_INCLUDED

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "class.hpp"
#include "struct.hpp"

namespace pyffi
{

namespace object_models
{

//class Expression;

class Expression
{
public:
	/*!
	 * Construct an expression from an expression string, where attributes
	 * in the given class may be referred to.
	 */
	Expression(const std::string & expr,
	           boost::shared_ptr<Class> class_)
		: class_(class_), left_index(-1), right_index(-1), left_value(0), right_value(0), oper(NONE) {};
	//! Evaluate the expression from the given structure.
	int eval(const Struct & struc) const {
		return 0;
	};
private:
	//! Expression operators.
	enum Operator { NONE, EQ, NEQ, AND, OR, BITAND, BITOR };
	//! Expression components.
	boost::shared_ptr<Class> class_;
	boost::scoped_ptr<Expression> left_expr;    //! The left expression.
	boost::scoped_ptr<Expression> right_expr;   //! The right expression.
	int left_index;           //! The index of the left value in class. -1 = use left_value instead.
	int right_index;          //! The index of the right value in class. -1 = use right_value instead.
	int left_value;           //! The left value.
	int right_value;          //! The right value.
	Operator oper;            //! The operator. NONE means use only the left side.
}; // class Expression

}; // namespace object_models

}; // namespace pyffi

#endif
