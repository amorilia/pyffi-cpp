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
#include <boost/weak_ptr.hpp>


#include "pyffi/object_models/map.hpp"
#include "pyffi/object_models/meta_attribute.hpp"

namespace pyffi
{

namespace object_models
{

// forward declaration
class MetaStruct;

//! Shared pointer to meta struct.
typedef boost::shared_ptr<MetaStruct> PMetaStruct;

//! Stores all information attached to a structure. This corresponds
//! essentially to the class declaration.
class MetaStruct
{
private:
	//! Private constructor to prevent it from being used. MetaStructs
	//! should be instantiated using one of the create static methods.
	MetaStruct() {};
public:
	//! Create top level structure declaration.
	static PMetaStruct create() {
		return PMetaStruct(new MetaStruct);
	};
	//! Create a structure declaration and add it to a parent.
	static PMetaStruct create(PMetaStruct parent, const std::string & name) {
		PMetaStruct child = create();
		child->parent = parent;
		parent->meta_structs_map.add(name, child);
		return child;
	};
	//! Create a structure declaration with base class and add it to a parent.
	static PMetaStruct create(PMetaStruct parent, const std::string & name, PMetaStruct base) {
		PMetaStruct child = create(parent, name);
		child->base = base;
		return child;
	};
	//! Add an attribute declaration.
	void add(const std::string & name, PMetaAttribute & p_meta_attribute) {
		// and store this index, using the name as key
		meta_attributes_index_map.add(name, meta_attributes.size());
		// store attribute
		meta_attributes.push_back(p_meta_attribute);
	};
	//! Get a structure declaration.
	PMetaStruct get(const std::string & name) {
		try {
			return meta_structs_map.get(name);
		} catch (const key_error &) {
			if (PMetaStruct p = parent.lock()) {
				return p->get(name);
			} else {
				throw name_error("MetaStruct has no attribute \"" + name + "\".");
			}
		};
	};
	//! Check subclass relationship.
	bool issubclass(PMetaStruct meta_struct) {
		if (meta_struct.get() == this) {
			return true;
		} else if (!base) {
			return false;
		} else {
			return base->issubclass(meta_struct);
		};
	};
private:
	//! Structure in which this structure is defined.
	boost::weak_ptr<MetaStruct> parent;
	//! Base from which the structure is derived.
	PMetaStruct base;
	//! Maps string name to their index as they have been added.
	Map<std::size_t> meta_attributes_index_map;
	//! List of attribute information as they have been added.
	std::vector<PMetaAttribute> meta_attributes;
	//! Map string name to meta struct children.
	Map<PMetaStruct> meta_structs_map;

	// allow Struct access to the internals
	friend class Struct;
};

}; // namespace object_models

}; // namespace pyffi

#endif
