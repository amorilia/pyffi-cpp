#include "Expression.hpp"
#include "MetaStruct.hpp"
#include "Struct.hpp"

// TODO
Expression::Expression(const std::string & expr, const MetaStruct & metastruct)
  : mpMetaStruct(&metastruct), mLeftIndex(-1), mRightIndex(-1),
    mLeftValue(0), mRightValue(0), mOp(NONE) {};

