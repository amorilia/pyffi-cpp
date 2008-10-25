#ifndef __STRUCT_HPP
#define __STRUCT_HPP

#include <vector>
#include "Attribute.hpp"
#include "MetaStruct.hpp"

/*!
 * A structure instance. This class serves to creates instance of a
 * MetaStruct.
 */
class Struct {
public:
  //! Create structure from a metaclass description.
  Struct(const MetaStruct & metastruct);
  //! Get a reference to the value of an attribute.
  Attribute & operator[](const std::string & name);
private:
  const MetaStruct * mpMetaStruct;
  std::vector<Attribute> mAttrs;
};

#endif

