/*!
 * Base class for all data that can be stored. Consists of read and write.
 */

#include <iostream>

namespace pyffi {

class AnyType {
	//! Read data from stream (TODO: with parameters taken from namespace).
	virtual void read(std::istream & in) = 0;
	//! Write data to stream (TODO: with parameters taken from namespace).
	virtual void write(std::ostream & out) const = 0;
}; // class AnyType

}; // namespace pyffi
