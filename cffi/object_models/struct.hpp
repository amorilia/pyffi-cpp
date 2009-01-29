#ifndef __STRUCT_HPP
#define __STRUCT_HPP

#include <typeinfo>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "attribute.hpp"
#include "meta_struct.hpp"

//! Shared pointer to meta struct.
typedef boost::shared_ptr<MetaStruct> PMetaStruct;

/*!
 * A structure instance. This class serves to creates instance of a
 * MetaStruct.
 */
class Struct {
public:
  //! Create structure from a metaclass description.
  Struct(PMetaStruct meta_struct)
	: meta_struct(meta_struct) {
	for (std::vector<MetaAttribute>::const_iterator
		   meta_attribute = meta_struct->meta_attributes.begin();
		 meta_attribute != meta_struct->meta_attributes.end();
		 meta_attribute++) {
	  // push back a copy of the default value
	  attributes.push_back(Attribute(meta_attribute->default_value));
	};
  };
  //! Get the value of an attribute.
  template<typename ValueType> ValueType & get(const std::string & name) {
	std::map<std::string, unsigned int>::iterator index = meta_struct->index_map.find(name);
	if (index != meta_struct->index_map.end()) {
	  try
	  {
	    return boost::any_cast<ValueType &>(attributes[index->second]);
	  }
	  catch (const boost::bad_any_cast &)
	  {
		throw type_error("Type mismatch on attribute \"" + name + "\" (required " + std::string(attributes[index->second].type().name()) + " but got " + std::string(typeid(ValueType).name()) + ").");
	  }
	} else {
	  throw name_error("Struct has no attribute \"" + name + "\".");
	};
  };
private:
  PMetaStruct meta_struct;
  std::vector<Attribute> attributes;
};

#endif

