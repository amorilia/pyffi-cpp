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
#include "pyffi/object_models/object.hpp"

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
	static PClass scope() {
		return PClass(new Class);
	};
	//! Create a nested class.
	PClass class_(const std::string & name) {
		PClass child = scope();
		child->parent = shared_from_this();
		class_map.add(name, child);
		return child;
	};
	//! Create a nested class with given base class.
	PClass class_(const std::string & name, PClass base) {
		PClass child = class_(name);
		child->base = base;
		return child;
	};
	//! Create a class attribute.
	template<typename ValueType>
	void def(const std::string & name, const ValueType & default_value) {
		// store index, using the name as key
		attrs_index_map.add(name, attrs.size());
		// store attribute
		attrs.push_back(Object(default_value));
	};
	//! Get a nested class declaration.
	PClass attr(const std::string & name) {
		try {
			return class_map.get(name);
		} catch (const key_error &) {
			if (PClass p = parent.lock()) {
				return p->attr(name);
			} else {
				throw name_error("Class \"" + name + "\" not found.");
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
	//! Class in which this class is nested.
	boost::weak_ptr<Class> parent;
	//! Base class from which the class is derived.
	boost::weak_ptr<Class> base;
	//! Maps names to attribute index.
	Map<std::size_t> attrs_index_map;
	//! List of attributes.
	std::vector<Object> attrs;
	//! Map names to nested classes.
	Map<PClass> class_map;

	// allow Instance access to the internals
	friend class Instance;
};

}; // namespace object_models

}; // namespace pyffi

#endif // PYFFI_OM_CLASS_HPP_INCLUDED
