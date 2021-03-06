/*

Copyright (c) 2007-2010, Python File Format Interface
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

#ifndef PYFFI_OM_MAP_HPP_INCLUDED
#define PYFFI_OM_MAP_HPP_INCLUDED

#include <utility> // pair

#include <boost/unordered_map.hpp>

#include "pyffi/exceptions.hpp"

namespace pyffi
{

namespace object_models
{

template <typename T>
class Map : private boost::unordered_map<std::string, T>
{
public:
	//! Register a new name, and a default value for it.
	void add(const std::string & name, const T & value) {
		std::pair<typename Map<T>::iterator, bool> ret = this->insert(std::make_pair(name, value));
		if (!ret.second) {
			// insert failed
			throw value_error("key '" + name + "' already added.");
		};
	};

	//! Get reference to value by name.
	T & get(const std::string & name) {
		typename Map<T>::iterator it = this->find(name);
		if (it != this->end()) {
			return it->second;
		} else {
			// if there is no name key, we signal this as a key error
			throw key_error("no key '" + name + "'.");
		};
	};
}; // class Map

}; // namespace object_models

}; // namespace pyffi

#endif
