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

#ifndef PYFFI_OM_METASTRUCT_HPP_INCLUDED
#define PYFFI_OM_METASTRUCT_HPP_INCLUDED

#include <vector>


#include "pyffi/object_models/map.hpp"
#include "pyffi/object_models/meta_attribute.hpp"

namespace pyffi
{

namespace object_models
{

//! Stores all information attached to a structure.
class MetaStruct
{
public:
	//! Size.
	typedef std::vector<MetaAttribute>::size_type size_type;
	//! Add an attribute to the structure.
	void add(const std::string & name, PMetaAttribute & p_meta_attribute) {
		// and store this index, using the name as key
		index_map.add(name, meta_attributes.size());
		// store attribute
		meta_attributes.push_back(p_meta_attribute);
	};
	size_type index(const std::string & name) {
		return index_map.get(name);
	};
private:
	//! Maps string name to their index as they have been added.
	Map<size_type> index_map;
	//! List of attribute information as they have been added.
	std::vector<PMetaAttribute> meta_attributes;

	// allow Struct access to the internals
	friend class Struct;
};

//! Shared pointer to meta struct.
typedef boost::shared_ptr<MetaStruct> PMetaStruct;

}; // namespace object_models

}; // namespace pyffi

#endif
