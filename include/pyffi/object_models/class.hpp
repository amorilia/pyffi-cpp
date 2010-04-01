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

#include <boost/enable_shared_from_this.hpp>
#include <boost/foreach.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

#include "pyffi/object_models/map.hpp"
#include "pyffi/object_models/object.hpp"

namespace pyffi
{

namespace object_models
{

//! A class for dynamic class declarations. Stores attribute
//! names, default attribute values, base class, and parent
//! class (for nesting).
class Class : public boost::enable_shared_from_this<Class>
{
public:
	typedef boost::shared_ptr<Class> PClass;

	//! Create top level class to contain other declarations.
	static PClass class_() {
		return PClass(new Class);
	};

	//! Create a nested class.
	PClass class_(const std::string & name) {
		PClass child = class_();
		child->parent = shared_from_this();
		class_map.add(name, child);
		return child;
	};

	//! Create a nested class with given base class.
	PClass class_(const std::string & name, PClass base) {
		PClass child = class_(name);
		child->base = base;
		// copy base class attributes
		child->attr_set = base->attr_set;
		// copy value
		child->value = value;
		return child;
	};

	//! Declare class as a primitive one, with given default value.
	void primitive(Object val) {
		value = val;
	};

	//! Get a nested class declaration.
	PClass get_class(const std::string & name) {
		try {
			return class_map.get(name);
		} catch (const key_error &) {
			if (PClass p = parent.lock()) {
				return p->get_class(name);
			} else {
				throw name_error("Class \"" + name + "\" not found.");
			}
		};
	};

	//! Create an attribute definition.
	void def(const std::string & name, PClass class_) {
		// create attribute
		Attribute attr(name, class_, attr_set.size());
		// store attribute
		std::pair<AttributeSetByName::iterator, bool> result = attr_set.get<by_name>().insert(attr);
		// check that it was succesfull
		if (!result.second) {
			throw value_error("Attribute \"" + name + "\" already defined.");
		};
	};

	//! Get the class and index of an attribute definition.
	std::pair<PClass, std::size_t> get_def(const std::string & name) {
		AttributeSetByName & attr_set_by_name = attr_set.get<by_name>();
		AttributeSetByName::iterator it = attr_set_by_name.find(name);
		if (it == attr_set_by_name.end()) {
			throw name_error("Attribute \"" + name + "\" not found.");
		};
		return std::make_pair(it->class_, it->index);
	};

	//! Check subclass relationship.
	bool is_subclass(PClass class_) {
		if (class_.get() == this) {
			return true;
		} else if(PClass b = base.lock()) {
			return b->is_subclass(class_);
		} else {
			return false;
		};
	};

	//! Instantiate class.
	Object instance() const {
		if (attr_set.empty()) {
			// primitive type: simple instantiation
			return Object(value);
		} else {
			// composite type: instantiate a vector containing instances
			// of each attribute class
			std::vector<Object> attrs;
			BOOST_FOREACH(const Attribute & attr, attr_set.get<by_index>()) {
				attrs.push_back(attr.class_->instance());
			};
			return Object(attrs);
		};
	};

private:
	//! Private constructor to prevent it from being
	//! used. Classes should be created using the class_
	//! methods.
	Class() {};

	struct Attribute {
		std::string name;
		PClass class_;
		std::size_t index;
		Attribute(const std::string & name, PClass class_, std::size_t index)
			: name(name), class_(class_), index(index) {};
	};

	struct by_name {};
	struct by_index {};

	typedef boost::multi_index_container<
	Attribute,
	boost::multi_index::indexed_by<
	// ordered by insertion (which is the same as ordered by index)
	boost::multi_index::sequenced<boost::multi_index::tag<by_index> >,
	// hashed by name
	boost::multi_index::hashed_unique<
	boost::multi_index::tag<by_name>,
	boost::multi_index::member<Attribute, std::string, &Attribute::name> >
	>
	> AttributeSet;

	typedef AttributeSet::index<by_index>::type AttributeSetByIndex;
	typedef AttributeSet::index<by_name>::type AttributeSetByName;


	//! Class in which this class is nested.
	boost::weak_ptr<Class> parent;

	//! Base class from which the class is derived.
	boost::weak_ptr<Class> base;

	//! Default value when instantiating the class if attr_set is
	//! empty.
	Object value;

	//! Attributes (name, class, and index).
	AttributeSet attr_set;

	//! Map names to nested classes.
	Map<PClass> class_map;
}; // class Class

//! For convenience.
typedef Class::PClass PClass;

}; // namespace object_models

}; // namespace pyffi

#endif // PYFFI_OM_CLASS_HPP_INCLUDED
