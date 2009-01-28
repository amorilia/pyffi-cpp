#ifndef __ATTRIBUTE_HPP
#define __ATTRIBUTE_HPP

#include <boost/any.hpp>

#include "../Exceptions.hpp"

/*!
 * A boost::any variant which can contain any type
 * but that cannot change type once created.
 * All attributes of a Struct have this type.
 */
class Attribute : public boost::any {
public:
  //! Constructor, sets the type and the value.
  template<typename ValueType> Attribute(const ValueType & value)
    : boost::any(value) {};
  //! Override assignment operator so type cannot be changed.
  Attribute & operator=(const Attribute & attr) {
    if (this->type() != attr.type())
      throw type_error("Type mismatch on attribute assignment.");
    boost::any::operator=(attr);
    return *this;
  };
  //! Override assignment operator so type cannot be changed.
  template<typename ValueType> Attribute & operator=(const ValueType & value) {
    if (this->type() != typeid(value))
      throw type_error("Type mismatch on attribute assignment.");
    boost::any::operator=(value);
    return *this;
  };
  //! Quick cast function.
  template<typename ValueType> ValueType get() {
    return boost::any_cast<ValueType>(*this);
  };
};

#endif

