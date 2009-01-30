#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <boost/any.hpp>

#include "../exceptions.hpp"

/*!
 * A boost::any variant which can contain any type but that cannot
 * change type once created. All objects (i.e. any data that can take
 * particular values) have this type.
 */
class Object : public boost::any {
public:
  //! Constructor, sets the type and the value.
  template<typename ValueType> Object(const ValueType & value)
    : boost::any(value) {};
  //! Override assignment operator so type cannot be changed.
  Object & operator=(const Object & obj) {
    if (this->type() != obj.type())
      throw type_error("Type mismatch on assignment (required " + std::string(this->type().name()) + " but got " + std::string(obj.type().name()) + ").");
    boost::any::operator=(obj);
    return *this;
  };
  //! Override assignment operator so type cannot be changed.
  template<typename ValueType> Object & operator=(const ValueType & value) {
    if (this->type() != typeid(value))
      throw type_error("Type mismatch on assignment (required " + std::string(this->type().name()) + " but got " + std::string(typeid(value).name()) + ").");
    boost::any::operator=(value);
    return *this;
  };
};

#endif

