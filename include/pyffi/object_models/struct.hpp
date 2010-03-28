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

#ifndef PYFFI_OM_INSTANCE_HPP_INCLUDED
#define PYFFI_OM_INSTANCE_HPP_INCLUDED

#include <typeinfo>
#include <vector>
#include <boost/foreach.hpp>

#include "object.hpp"
#include "class.hpp"

namespace pyffi
{

namespace object_models
{

/*!
 * A structure instance. This class serves to create an instance of a
 * Class.
 */
class Instance
{
public:
	//! Create structure from a meta struct description.
	Instance(PClass class_)
		: class_(class_) {
		BOOST_FOREACH(Object attr, class_->attrs) {
			// push back a copy of the default value
			objects.push_back(attr);
		};
	}
	//! Copy constructor.
	Instance(const Instance & struc)
		: class_(struc.class_), objects(struc.objects) {};
	//! Get reference to the value of an attribute.
	template<typename ValueType> ValueType & attr(const std::string & name) {
		std::size_t index;

		// search name in meta struct index
		try {
			index = class_->attrs_index_map.get(name);
		} catch (const key_error &) {
			// not found, so throw an exception
			throw name_error("Instance has no attribute \"" + name + "\".");
		};

		// return attribute as requested type
		try {
			return objects[index].get<ValueType>();
		} catch (const type_error &) {
			// could not get attribute in requested type, so throw exception
			throw type_error("Type mismatch on attribute \"" + name + "\" (required " + std::string(typeid(objects[index]).name()) + " but got " + std::string(typeid(ValueType).name()) + ").");
		};

	};
private:
	//! Class information (list of attribute types, default values, ...).
	PClass class_;
	//! List of attribute values.
	std::vector<Object> objects;
}; // class Instance

}; // namespace object_models

}; // namespace pyffi

#endif // PYFFI_OM_INSTANCE_HPP_INCLUDED
