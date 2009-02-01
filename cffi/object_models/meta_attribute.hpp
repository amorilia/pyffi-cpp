#ifndef __METAATTRIBUTE_HPP
#define __METAATTRIBUTE_HPP

#include <vector>
#include <boost/shared_ptr.hpp>

#include "object.hpp"

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
};

#endif
