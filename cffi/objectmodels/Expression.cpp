#include "Expression.hpp"
#include "MetaStruct.hpp"
#include "Struct.hpp"

// TODO
Expression::Expression(const std::string & expr,
					   boost::shared_ptr<MetaStruct> meta_struct)
  : meta_struct(meta_struct), left_index(-1), right_index(-1),
    left_value(0), right_value(0), oper(NONE) {};

int Expression::eval(const Struct & struc) const {
  return 0;
};
