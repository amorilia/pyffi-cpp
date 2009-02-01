#ifndef __STRUCT_HPP
#define __STRUCT_HPP

#include <typeinfo>
#include <vector>
#include <boost/foreach.hpp>

#include "object.hpp"
#include "meta_struct.hpp"

namespace pyffi {

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
		std::map<std::string, unsigned int>::iterator index;

		// search name in meta struct index
		index = meta_struct->index_map.find(name);

		if (index != meta_struct->index_map.end()) {
			// found, so return attribute as requested type
			try {
				return objects[index->second].get<ValueType>();
			} catch (const type_error &) {
				// could not get attribute in requested type, so throw exception
				throw type_error("Type mismatch on attribute \"" + name + "\" (required " + std::string(typeid(objects[index->second]).name()) + " but got " + std::string(typeid(ValueType).name()) + ").");
			}
		} else {
			// not found, so throw an exception
			throw name_error("Struct has no attribute \"" + name + "\".");
		};
	};
private:
	//! Metaclass information (list of attribute types, default values, ...).
	PMetaStruct meta_struct;
	//! List of attribute values.
	std::vector<Object> objects;
}; // class Struct

}; // namespace pyffi

#endif // __STRUCT_HPP
