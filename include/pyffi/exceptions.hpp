#ifndef __EXCEPTIONS_HPP
#define __EXCEPTIONS_HPP

#include <stdexcept>

namespace pyffi {

//! Thrown on name mismatch.
class name_error : public std::runtime_error {
public:
	name_error(const std::string & msg)
			: std::runtime_error(msg) {};
};

//! Thrown on type mismatch.
class type_error : public std::runtime_error {
public:
	type_error(const std::string & msg)
			: std::runtime_error(msg) {};
};

//! Thrown on value mismatch.
class value_error : public std::runtime_error {
public:
	value_error(const std::string & msg)
			: std::runtime_error(msg) {};
};

}; // namespace pyffi

#endif
