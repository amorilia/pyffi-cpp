#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <boost/any.hpp>

#include "cffi/exceptions.hpp"

/*!
 * A boost::any variant which can contain any type but that cannot
 * change type once created. All objects (i.e. any data that can take
 * particular values) have this type.
 */
class Object {
public:
  //! Default constructor is disabled.
  //Object() {
  //  throw type_error("Object must be constructed with a specific value.");
  //};
  //! Copy constructor.
  Object(const Object & object)
    : m_value(object.m_value) {};
  //! Constructor to set the type and the value.
  template<typename ValueType> Object(const ValueType & value)
    : m_value(value) {};
  //! Get reference to value stored in the object.
  template<typename ValueType> ValueType & get() {
    try {
      return boost::any_cast<ValueType &>(m_value);
    } catch (const boost::bad_any_cast &) {
      throw  type_error("Type mismatch on object get (required " + std::string(m_value.type().name()) + " but got " + std::string(typeid(ValueType).name()) + ").");
    };
  };
  //! Override assignment operator so type cannot be changed.
  Object & operator=(const Object & obj) {
    if (m_value.type() != obj.m_value.type())
      throw type_error("Type mismatch on object assignment (required " + std::string(m_value.type().name()) + " but got " + std::string(obj.m_value.type().name()) + ").");
    m_value = obj.m_value;
    return *this;
  };
  //! Override assignment operator so type cannot be changed.
  template<typename ValueType> Object & operator=(const ValueType & value) {
    if (m_value.type() != typeid(value)) {
      throw type_error("Type mismatch on value assignment (required " + std::string(m_value.type().name()) + " but got " + std::string(typeid(value).name()) + ").");
    };
    m_value = value;
    return *this;
  };
private:
  boost::any m_value;
};

#endif

