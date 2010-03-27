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

#ifndef PYFFI_OM_FILE_FORMAT_HPP_INCLUDED
#define PYFFI_OM_FILE_FORMAT_HPP_INCLUDED

#include <list>

#include "pyffi/exceptions.hpp"
#include "pyffi/object_models/object_map.hpp"
/* TODO
#include "pyffi/object_models/class.hpp"
#include "pyffi/object_models/struct.hpp"
*/

namespace pyffi
{

namespace object_models
{

//! Stores all information attached to a format.
class FileFormat : private ObjectMap
{
public:
	//! The file extensions of the format (nif, cgf, tga, dds, ...)
	std::list<std::string> extensions;
	FileFormat() : extensions() {};
	/* TODO
		//! Add a structure declaration.
		PClass struct_(const std::string & name) {
			// create and store the meta struct
			PClass class_(new Class);
			ObjectMap::add(class_);
			return class_;
		};
		//! Instantiate a structure.
		Struct instantiate(const std::string & name) {
			return Struct(ObjectMap::get<PClass>(name));
		};
	*/
}; // class FileFormat

}; // namespace object_models

}; // namespace pyffi

#endif
