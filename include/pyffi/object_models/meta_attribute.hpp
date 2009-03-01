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

#ifndef PYFFI_OM_METAATTRIBUTE_HPP_INCLUDED
#define PYFFI_OM_METAATTRIBUTE_HPP_INCLUDED

#include <vector>
#include <boost/shared_ptr.hpp>

#include "object.hpp"

namespace pyffi {

namespace object_models {

class Expression;

/*!
 * The MetaAttribute class stores information about the attributes of a
 * structure.
 */
class MetaAttribute {
public:
	/*!
	 * Initialize information about the attribute: type and default value.
	 */
	template<typename ValueType>
	MetaAttribute(const ValueType & default_value)
			: default_value(default_value), shape() {};
	template<typename ValueType>
	MetaAttribute(const ValueType & default_value, const std::vector<boost::shared_ptr<Expression> > & shape)
			: default_value(default_value), shape(shape) {};
private:
	/*!
	 * default_value plays both the role of storing the type of the
	 * attribute, as well as storing the default value.
	 */
	Object default_value;
	//! Dimensions of array (this is an empty list for non-array's).
	std::vector<boost::shared_ptr<Expression> > shape;

	// Struct can access the internal data.
	friend class Struct;
}; // class MetaAttribute

typedef boost::shared_ptr<MetaAttribute> PMetaAttribute;

}; // namespace object_models

}; // namespace pyffi

#endif
