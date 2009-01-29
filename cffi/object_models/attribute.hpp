#ifndef __ATTRIBUTE_HPP
#define __ATTRIBUTE_HPP

#include <boost/any.hpp>

#include "../exceptions.hpp"

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
      throw type_error("Type mismatch on assignment (required " + std::string(this->type().name()) + " but got " + std::string(attr.type().name()) + ").");
    boost::any::operator=(attr);
    return *this;
  };
  //! Override assignment operator so type cannot be changed.
  template<typename ValueType> Attribute & operator=(const ValueType & value) {
    if (this->type() != typeid(value))
      throw type_error("Type mismatch on assignment (required " + std::string(this->type().name()) + " but got " + std::string(typeid(value).name()) + ").");
    boost::any::operator=(value);
    return *this;
  };
};

#endif

