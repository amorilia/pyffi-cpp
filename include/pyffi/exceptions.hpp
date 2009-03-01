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

#ifndef __PYFFI_EXCEPTIONS_HPP_INCLUDED
#define __PYFFI_EXCEPTIONS_HPP_INCLUDED

#include <stdexcept>

namespace pyffi {

//! Thrown on runtime error (convenience wrapper around std::runtime_error).
class runtime_error : public std::runtime_error {
public:
	runtime_error(const std::string & msg)
			: std::runtime_error(msg) {};
};

//! Thrown on name mismatch.
class name_error : public runtime_error {
public:
	name_error(const std::string & msg)
			: runtime_error(msg) {};
};

//! Thrown on type mismatch.
class type_error : public runtime_error {
public:
	type_error(const std::string & msg)
			: runtime_error(msg) {};
};

//! Thrown on value mismatch.
class value_error : public runtime_error {
public:
	value_error(const std::string & msg)
			: runtime_error(msg) {};
};

//! Thrown on attribute mismatch.
class attribute_error : public runtime_error {
public:
	attribute_error(const std::string & msg)
			: runtime_error(msg) {};
};

//! Thrown on key mismatch.
class key_error : public runtime_error {
public:
	key_error(const std::string & msg)
			: runtime_error(msg) {};
};

//! Thrown on input/output error.
class io_error : public runtime_error {
public:
	io_error(const std::string & msg)
			: runtime_error(msg) {};
};

}; // namespace pyffi

#endif
