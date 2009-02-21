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

#ifndef PYFFI_MAP_HPP_INCLUDED
#define PYFFI_MAP_HPP_INCLUDED

// unordered_map introduced in boost 1.36.0
#if BOOST_VERSION >= 13600
#include <boost/unordered_map.hpp>
#else
#include <map>
#endif

namespace pyffi {

// unordered_map introduced in boost 1.36.0
#if BOOST_VERSION >= 13600
// special very fast name hash function (assuming names start with different
// letters in most occasions)
struct name_hash : std::unary_function<std::string, std::size_t> {
	std::size_t operator()(std::string const & name) const {
		std::size_t seed = 0;
		try {
			boost::hash_combine(seed, name.at(0));
			//boost::hash_combine(seed, name.at(1));
		} catch (const & std::out_of_range) {
			// pass the exception
		};
		return seed;
	}
};

template <typename T>
class Map : public boost::unordered_map<std::string, T, name_hash> {};
#else
template <typename T>
class Map : public std::map<std::string, T> {};
#endif

} // namespace pyff

#endif
