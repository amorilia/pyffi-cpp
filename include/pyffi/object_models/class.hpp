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

#ifndef PYFFI_OM_CLASS_HPP_INCLUDED
#define PYFFI_OM_CLASS_HPP_INCLUDED

#include <vector>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


#include "pyffi/object_models/map.hpp"
#include "pyffi/object_models/meta_attribute.hpp"

namespace pyffi
{

namespace object_models
{

// forward declaration
class Class;

//! Shared pointer to meta struct.
typedef boost::shared_ptr<Class> PClass;

//! Stores all information attached to a structure. This corresponds
//! essentially to the class declaration.
class Class : public boost::enable_shared_from_this<Class>
{
private:
	//! Private constructor to prevent it from being used. Classes
	//! should be instantiated using the create and class_ methods.
	Class() {};
public:
	//! Create top level namespace to contain declarations.
	static PClass create() {
		return PClass(new Class);
	};
	//! Create a child class declaration.
	PClass class_(const std::string & name) {
		PClass child = create();
		child->parent = shared_from_this();
		classes_map.add(name, child);
		return child;
	};
	//! Create a child class declaration with base class.
	PClass class_(const std::string & name, PClass base) {
		PClass child = class_(name);
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
	PClass get(const std::string & name) {
		try {
			return classes_map.get(name);
		} catch (const key_error &) {
			if (PClass p = parent.lock()) {
				return p->get(name);
			} else {
				throw name_error("Class has no attribute \"" + name + "\".");
			}
		};
	};
	//! Check subclass relationship.
	bool issubclass(PClass class_) {
		if (class_.get() == this) {
			return true;
		} else if(PClass b = base.lock()) {
			return b->issubclass(class_);
		} else {
			return false;
		};
	};
private:
	//! Structure in which this structure is defined.
	boost::weak_ptr<Class> parent;
	//! Base from which the structure is derived.
	boost::weak_ptr<Class> base;
	//! Maps string name to their index as they have been added.
	Map<std::size_t> meta_attributes_index_map;
	//! List of attribute information as they have been added.
	std::vector<PMetaAttribute> meta_attributes;
	//! Map string name to meta struct children.
	Map<PClass> classes_map;

	// allow Struct access to the internals
	friend class Struct;
};

}; // namespace object_models

}; // namespace pyffi

#endif
