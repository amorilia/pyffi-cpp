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

//! Contains an instance of a class.
class Instance
{
public:
	//! Stores all information attached to a structure. This corresponds
	//! essentially to the class declaration.
	class Class : public boost::enable_shared_from_this<Class>
	{
	private:
		//! Private constructor to prevent it from being used. Classes
		//! should be created using the scope and class_ methods.
		Class() {};
	public:
		//! Callable shared pointer to class.
		class PClass : public boost::shared_ptr<Class>
		{
		public:
			PClass() : boost::shared_ptr<Class>() {};
			PClass(Class *ptr) : boost::shared_ptr<Class>(ptr) {};
			PClass(const boost::shared_ptr<Class> & ptr) : boost::shared_ptr<Class>(ptr) {};
			Instance operator ()() {
				return (**this)();
			};
		};

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
		//! Get reference to a class attribute.
		//! Implementation note: specialized for ValueType = PClass.
		template<typename ValueType>
		ValueType & attr(const std::string & name) {
			std::size_t index;
			try {
				index = attrs_index_map.get(name);
			} catch (const key_error &) {
				throw name_error("Attribute \"" + name + "\" not found.");
			};
			return attrs[index].get<ValueType>();
		};

		// cannot define template here: the standard says that the
		// specialization must be defined in the namespace in which
		// the class is defined, so the specialization is further
		// below

		//template<>
		//PClass & attr<PClass>(const std::string & name) { ... };

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
		//! Instantiate class.
		Instance operator()() {
			return Instance(shared_from_this());
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
	}; // class Class

	//! For convenience.
	typedef Class::PClass PClass;

	//! Get reference to the value of an attribute.
	template<typename ValueType> ValueType & attr(const std::string & name) {
		std::size_t index;
		if (PClass c = class_.lock()) {
			// search name in class attribute index map
			try {
				index = c->attrs_index_map.get(name);
			} catch (const key_error &) {
				// not found, so throw an exception
				throw name_error("Instance has no attribute \"" + name + "\".");
			};
		} else {
			throw runtime_error("Instance has no class, and can therefore not be instantiated.");
		};


		// return attribute as requested type
		try {
			return attrs[index].get<ValueType>();
		} catch (const type_error &) {
			// could not get attribute in requested type, so throw exception
			throw type_error("Type mismatch on attribute \"" + name + "\" (required " + std::string(typeid(attrs[index]).name()) + " but got " + std::string(typeid(ValueType).name()) + ").");
		};

	};

private:

	//! Create instance of a class. This is private, use the
	//! Instance::Class::operator() to instantiate.
	Instance(PClass class_)
		: class_(class_), attrs(class_->attrs) {};
	//! Class information (list of attribute types, default values, ...).
	boost::weak_ptr<Class> class_;
	//! List of attribute values.
	std::vector<Object> attrs;
}; // class Instance

//! For convenience.
typedef Instance::Class Class;

//! For convenience.
typedef Instance::PClass PClass;

//! Get a nested class declaration.
template<>
PClass & Instance::Class::attr<PClass>(const std::string & name)
{
	try {
		return class_map.get(name);
	} catch (const key_error &) {
		if (PClass p = parent.lock()) {
			return p->attr<PClass>(name);
		} else {
			throw name_error("Class \"" + name + "\" not found.");
		}
	};
};

}; // namespace object_models

}; // namespace pyffi

#endif // PYFFI_OM_CLASS_HPP_INCLUDED
