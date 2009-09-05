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

#include <iostream>
#include <boost/cstdint.hpp>
#include <boost/integer_traits.hpp>

#include "pyffi/object_models/any_type.hpp"
#include "pyffi/object_models/object.hpp"

namespace pyffi {

namespace object_models {

/*!
 * Base class for integer data.
 */

template<typename IntegralType>
class IntType : public pyffi::object_models::AnyType {
public:
	IntType() : AnyType((IntegralType)0) {};

	//! Override assignment operator for convenience.
	IntType & operator=(const Object & obj) {
		Object::operator=(obj);
		return *this;
	};
	//! Get reference to value stored in the object.
	IntegralType & get() {
		// should not throw!
		return boost::any_cast<IntegralType &>(m_value);
	};
	//! Get const reference to value stored in the object.
	const IntegralType & get() const {
		// should not throw!
		return boost::any_cast<const IntegralType &>(m_value);
	};
	//! Assignment (from any integral type).
	template<typename ValueType> IntType & operator=(const ValueType & value) {
		if (boost::integer_traits<ValueType>::is_integral) {
			if ((value >= boost::integer_traits<IntegralType>::const_min) && (value <= boost::integer_traits<IntegralType>::const_max)) {
				Object::operator=((IntegralType)value);
			} else {
				// raise value error
				std::stringstream ss;
				ss << "Value out of range (required between " << boost::integer_traits<IntegralType>::const_min << " and " << boost::integer_traits<IntegralType>::const_max << " but got " << value << ").";
				throw value_error(ss.str());
			}
		} else {
			// raise type error
			throw type_error("Type mismatch on value assignment (required integral type but got " + std::string(typeid(value).name()) + ").");
		}
		return *this;
	};

	//! Read data from stream (TODO: with parameters taken from namespace).
	void read(std::istream & in) {
		IntegralType buf;
		in.read((char *)&buf, sizeof(buf));
		get() = buf;
	};

	//! Write data to stream (TODO: with parameters taken from namespace).
	void write(std::ostream & out) const {
		const IntegralType buf = get();
		out.write((const char *)&buf, sizeof(buf));
	};

}; // class AnyType

}; // namespace object_models

}; // namespace pyffi
