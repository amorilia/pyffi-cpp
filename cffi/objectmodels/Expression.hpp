#ifndef __EXPRESSION_HPP
#define __EXPRESSION_HPP

#include "MetaStruct.hpp"
#include "Struct.hpp"

class Expression;

class Expression {
public:
  /*!
   * Construct an expression from an expression string, where attributes
   * in the given metastruct may be referred to.
   */
  Expression(const std::string & expr, const MetaStruct & metastruct);
  //! Evaluate the expression from the given structure.
  int eval(const Struct & struc) const;
private:
  //! Expression operators.
  enum Operator { NONE, EQ, NEQ, AND, OR, BITAND, BITOR };
  //! Expression components.
  const MetaStruct * mpMetaStruct;
  Expression * mLeftExpr;   //! The left expression.
  Expression * mRightExpr;  //! The right expression.
  int mLeftIndex;           //! The index of the left value in metastruct. -1 = use mLeftValue instead.
  int mRightIndex;          //! The index of the right value in metastruct. -1 = use mRightValue instead.
  int mLeftValue;           //! The left value.
  int mRightValue;          //! The right value.
  Operator mOp;             //! The operator. NONE means use only the left side.
};

#endif

