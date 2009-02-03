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

#ifndef PYFFI_OBJECT_HPP_INCLUDED
#define PYFFI_OBJECT_HPP_INCLUDED

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include "pyffi/exceptions.hpp"

namespace pyffi {

/*!
 * A boost::any variant which can contain any type but that cannot
 * change type once created. All objects (i.e. any data that can take
 * particular values) have this type.
 */
class Object {
public:
	//! Default constructor throws.
	Object() {
		throw type_error("Object must be constructed with a specific value.");
	};
	//! Copy constructor.
	Object(const Object & object)
			: m_value(object.m_value) {};
	//! Constructor to set the type and the value.
	template<typename ValueType> Object(const ValueType & value)
			: m_value(value) {};
	//! Get reference to value stored in the object.
	template<typename ValueType> ValueType & get() {
		try {
			return boost::any_cast<ValueType &>(m_value);
		} catch (const boost::bad_any_cast &) {
			throw type_error("Type mismatch on object get (required " + std::string(m_value.type().name()) + " but got " + std::string(typeid(ValueType).name()) + ").");
		};
	};
	//! Override assignment operator so type cannot be changed.
	Object & operator=(const Object & obj) {
		if (m_value.type() != obj.m_value.type())
			throw type_error("Type mismatch on object assignment (required " + std::string(m_value.type().name()) + " but got " + std::string(obj.m_value.type().name()) + ").");
		m_value = obj.m_value;
		return *this;
	};
	//! Override assignment operator so type cannot be changed.
	template<typename ValueType> Object & operator=(const ValueType & value) {
		if (m_value.type() != typeid(value)) {
			throw type_error("Type mismatch on value assignment (required " + std::string(m_value.type().name()) + " but got " + std::string(typeid(value).name()) + ").");
		};
		m_value = value;
		return *this;
	};
private:
	boost::any m_value;
}; // class Object

typedef boost::shared_ptr<Object> PObject;

}; // namespace pyffi

#endif

