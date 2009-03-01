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

#ifndef PYFFI_OM_STRUCT_HPP_INCLUDED
#define PYFFI_OM_STRUCT_HPP_INCLUDED

#include <typeinfo>
#include <vector>
#include <boost/foreach.hpp>

#include "object.hpp"
#include "meta_struct.hpp"

namespace pyffi {

namespace object_models {

/*!
 * A structure instance. This class serves to create an instance of a
 * MetaStruct.
 */
class Struct {
public:
	//! Create structure from a metaclass description.
	Struct(PMetaStruct meta_struct)
			: meta_struct(meta_struct) {
		BOOST_FOREACH(const MetaAttribute & meta_attribute, meta_struct->meta_attributes) {
			// push back a copy of the default value
			objects.push_back(Object(meta_attribute.default_value));
		};
	}
	//! Copy constructor.
	Struct(const Struct & struc)
			: meta_struct(struc.meta_struct), objects(struc.objects) {};
	//! Get reference to the value of an attribute.
	template<typename ValueType> ValueType & get_attr(const std::string & name) {
		std::size_t index;

		// search name in meta struct index
		try {
			index = meta_struct->index_map.get(name);
		} catch (const key_error &) {
			// not found, so throw an exception
			throw name_error("Struct has no attribute \"" + name + "\".");
		};

		// return attribute as requested type
		try {
			return objects[index->second].get<ValueType>();
		} catch (const type_error &) {
			// could not get attribute in requested type, so throw exception
			throw type_error("Type mismatch on attribute \"" + name + "\" (required " + std::string(typeid(objects[index->second]).name()) + " but got " + std::string(typeid(ValueType).name()) + ").");
		};

	};
private:
	//! Metaclass information (list of attribute types, default values, ...).
	PMetaStruct meta_struct;
	//! List of attribute values.
	std::vector<Object> objects;
}; // class Struct

}; // namespace object_models

}; // namespace pyffi

#endif // __STRUCT_HPP
