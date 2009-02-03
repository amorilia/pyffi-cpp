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

#ifndef PYFFI_ARGS_HPP_INCLUDED
#define PYFFI_ARGS_HPP_INCLUDED

#include <utility> // pair
// unordered_map introduced in boost 1.36.0
#if BOOST_VERSION >= 013600
#include <boost/unordered_map.hpp>
#else
#include <map>
#endif

#include "pyffi/exceptions.hpp"
#include "pyffi/object_models/object.hpp"

namespace pyffi {

/*!
 * A std::map<std::string, Object> variant with an interface that
 * matches the rest of the library.
 */
class Args {
public:
	//! Default constructor.
	Args() : m_map() {};
	//! Copy constructor.
	Args(const Args & args) : m_map(args.m_map) {};
	//! Constructor to set the type and the value.
	template<typename ValueType> void add(const std::string & name, const ValueType & value) {
		// if name not found, add object to the map
	  std::pair<ArgsMap::iterator, bool> ret = m_map.insert(make_pair(name, Object(value)));
		if (!ret.second) {
			// insert failed
			throw value_error("'" + name + "' already added to arguments.");
		};
	};
	//! Get reference to value stored in the object.
	template<typename ValueType> ValueType & get(const std::string & name) {
	  ArgsMap::iterator it = m_map.find(name);
	  if (it != m_map.end()) {
		return it->second.get<ValueType>();
	  } else {
			// if there is no name key, we signal this as a key error
			throw key_error("no argument with key '" + name + "'");
		};
	};
private:
// unordered_map introduced in boost 1.36.0
#if BOOST_VERSION >= 013600
	typedef boost::unordered_map<std::string, Object> ArgsMap;
#else
	typedef std::map<std::string, Object> ArgsMap;
#endif
	ArgsMap m_map;
}; // class Args

}; // namespace pyffi

#endif
