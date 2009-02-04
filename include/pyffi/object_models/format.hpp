#ifndef __FORMAT_HPP
#define __FORMAT_HPP

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "../exceptions.hpp"
#include "meta_struct.hpp"

namespace pyffi {

//! Stores all information attached to a format.
class Format {
public:
	//! Add an attribute to the structure.
	PMetaStruct add_struct(const std::string & name) {
		// calculate index of new meta struct
		// and store index
		index_map[name] = meta_structs.size();
		// create and store the meta struct
		PMetaStruct meta_struct(new MetaStruct);
		meta_structs.push_back(meta_struct);
		return meta_struct;
	};
	PMetaStruct get_struct(const std::string & name) {
		std::map<std::string, unsigned int>::const_iterator index
		= index_map.find(name);
		if (index != index_map.end()) {
			return meta_structs[index->second];
		} else {
			throw name_error("Format has no struct \"" + name + "\".");
		};
	}
private:
	//! Maps string name to their index as they have been added.
	std::map<std::string, unsigned int> index_map;
	//! List of meta structs as they have been added.
	std::vector<PMetaStruct> meta_structs;
}; // class Format

}; // namespace pyffi

#endif