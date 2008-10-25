#ifndef __ATTRIBUTE_HPP
#define __ATTRIBUTE_HPP

#include <stdexcept>
#include <boost/any.hpp>

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
      throw assign_error();
    boost::any::operator=(attr);
    return *this;
  };
  //! Override assignment operator so type cannot be changed.
  template<typename ValueType> Attribute & operator=(const ValueType & value) {
    if (this->type() != typeid(value))
      throw assign_error();
    boost::any::operator=(value);
    return *this;
  };
  //! Quick cast function.
  template<typename ValueType> ValueType get() {
    return boost::any_cast<ValueType>(*this);
  };
  //! Thrown on type mismatch during assignment.
  class assign_error : public std::runtime_error {
  public:
    assign_error()
      : std::runtime_error("invalid type on assignment") {};
  };
};

#endif

