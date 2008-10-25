#ifndef __METASTRUCT_HPP
#define __METASTRUCT_HPP

#include <vector>
#include <map>
#include "MetaAttribute.hpp"

//! Stores all information attached to a structure.
class MetaStruct {
public:
  //! Add an attribute to the structure.
  template<typename ValueType>
  void addAttribute(const std::string & name, const ValueType & default_value) {
    // calculate index of new attribute
    unsigned int index = mMetaAttrs.size();
    // store index
    mIndex[name] = index;
    // store attribute
    mMetaAttrs.push_back(MetaAttribute(default_value));
  };
private:
  //! Maps string name to their index as they have been added.
  std::map<std::string, unsigned int> mIndex;
  //! List of attribute information as they have been added.
  std::vector<MetaAttribute> mMetaAttrs;

  // allow Struct access to the internals
  friend class Struct;
};

#endif
