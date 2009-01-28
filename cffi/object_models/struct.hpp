#ifndef __STRUCT_HPP
#define __STRUCT_HPP

#include <vector>
#include <boost/shared_ptr.hpp>

#include "attribute.hpp"
#include "meta_struct.hpp"

/*!
 * A structure instance. This class serves to creates instance of a
 * MetaStruct.
 */
class Struct {
public:
  //! Create structure from a metaclass description.
  Struct(boost::shared_ptr<MetaStruct> meta_struct)
	: meta_struct(meta_struct) {
	for (std::vector<MetaAttribute>::const_iterator
		   meta_attribute = meta_struct->meta_attributes.begin();
		 meta_attribute != meta_struct->meta_attributes.end();
		 meta_attribute++) {
	  // push back a copy of the default value
	  attributes.push_back(Attribute(meta_attribute->default_value));
	};
  };
  //! Get a reference to the value of an attribute.
  Attribute & operator[](const std::string & name) {
	return attributes[meta_struct->index_map.find(name)->second];
  };
private:
  boost::shared_ptr<MetaStruct> meta_struct;
  std::vector<Attribute> attributes;
};

#endif

