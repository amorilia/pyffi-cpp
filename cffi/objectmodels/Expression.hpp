#ifndef __EXPRESSION_HPP
#define __EXPRESSION_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "MetaStruct.hpp"
#include "Struct.hpp"

//class Expression;

class Expression {
public:
  /*!
   * Construct an expression from an expression string, where attributes
   * in the given metastruct may be referred to.
   */
  Expression(const std::string & expr,
			 boost::shared_ptr<MetaStruct> meta_struct)
	: meta_struct(meta_struct), left_index(-1), right_index(-1),
	  left_value(0), right_value(0), oper(NONE) {};
  //! Evaluate the expression from the given structure.
  int eval(const Struct & struc) const {
	return 0;
  };
private:
  //! Expression operators.
  enum Operator { NONE, EQ, NEQ, AND, OR, BITAND, BITOR };
  //! Expression components.
  boost::shared_ptr<MetaStruct> meta_struct;
  boost::scoped_ptr<Expression> left_expr;    //! The left expression.
  boost::scoped_ptr<Expression> right_expr;   //! The right expression.
  int left_index;           //! The index of the left value in metastruct. -1 = use left_value instead.
  int right_index;          //! The index of the right value in metastruct. -1 = use right_value instead.
  int left_value;           //! The left value.
  int right_value;          //! The right value.
  Operator oper;            //! The operator. NONE means use only the left side.
};

#endif
