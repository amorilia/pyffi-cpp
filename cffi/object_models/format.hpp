#ifndef __FORMAT_HPP
#define __FORMAT_HPP

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "../exceptions.hpp"
#include "meta_struct.hpp"

//! Stores all information attached to a format.
class Format {
public:
  //! Add an attribute to the structure.
  boost::shared_ptr<MetaStruct> new_struct(const std::string & name) {
    // calculate index of new meta struct
    // and store index
    index_map[name] = meta_structs.size();
    // create and store the meta struct
	boost::shared_ptr<MetaStruct> meta_struct(new MetaStruct);
    meta_structs.push_back(meta_struct);
	return meta_struct;
  };
  boost::shared_ptr<MetaStruct> operator[](const std::string & name) {
	std::map<std::string, unsigned int>::const_iterator index
	  = index_map.find(name);
	if (index != index_map.end()) {
	  return meta_structs[index->second];
	} else {
	  throw name_error("Format has no struct '" + name + "'.");
	};
  }
private:
  //! Maps string name to their index as they have been added.
  std::map<std::string, unsigned int> index_map;
  //! List of meta structs as they have been added.
  std::vector<boost::shared_ptr<MetaStruct> > meta_structs;
};

#endif
