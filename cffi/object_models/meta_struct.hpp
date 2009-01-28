#ifndef __METASTRUCT_HPP
#define __METASTRUCT_HPP

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "MetaAttribute.hpp"

//! Stores all information attached to a structure.
class MetaStruct {
public:
  //! Add an attribute to the structure.
  template<typename ValueType>
  void add_attribute(const std::string & name,
					 const ValueType & default_value) {
    // calculate index of new attribute
    // and store this index, using the name as key
    index_map[name] = meta_attributes.size();
    // store attribute
    meta_attributes.push_back(MetaAttribute(default_value));
  };
private:
  //! Maps string name to their index as they have been added.
  std::map<std::string, unsigned int> index_map;
  //! List of attribute information as they have been added.
  std::vector<MetaAttribute> meta_attributes;

  // allow Struct access to the internals
  friend class Struct;
};

#endif
