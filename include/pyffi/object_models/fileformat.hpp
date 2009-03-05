/*

Copyright (c) 2007-2008, Python File Format Interface
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the Python File Format Interface
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef PYFFI_OM_FORMAT_HPP_INCLUDED
#define PYFFI_OM_FORMAT_HPP_INCLUDED

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "../exceptions.hpp"
#include "meta_struct.hpp"

namespace pyffi {

namespace object_models {

//! Stores all information attached to a format.
class FileFormat {
public:
	//! The name of the format (nif, cgf, tga, dds, ...)
	static string name = "";

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

}; // namespace object_models

}; // namespace pyffi

#endif
