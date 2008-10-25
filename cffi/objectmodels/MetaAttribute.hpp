#ifndef __METAATTRIBUTE_HPP
#define __METAATTRIBUTE_HPP

#include "Attribute.hpp"

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
    : mDefault(default_value), mVer1(-1), mVer2(-1), mUserVer(-1) {};
  template<typename ValueType>
  MetaAttribute(const ValueType & default_value, int ver1, int ver2, int userver, const Expression & arr1, const Expression & arr2, const Expression & cond)
    : mDefault(default_value), mVer1(ver1), mVer2(ver2), mUserVer(userver) {};
private:
  /*!
   * mDefault plays both the role of storing the type of the
   * attribute, as well as storing the default value.
   */
  Attribute mDefault;
  int mVer1;
  int mVer2;
  int mUserVer;
  const Expression * mArr1;
  const Expression * mArr2;
  const Expression * mCond;

  // Struct can access the internal data.
  friend class Struct;
};

#endif

