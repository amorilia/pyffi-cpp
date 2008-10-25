#include "Struct.hpp"

Struct::Struct(const MetaStruct & metastruct) {
  mpMetaStruct = & metastruct;
  for (std::vector<MetaAttribute>::const_iterator metaattr = metastruct.mMetaAttrs.begin();
       metaattr != metastruct.mMetaAttrs.end(); metaattr++) {
    // push back a copy of the default value
    mAttrs.push_back(Attribute(metaattr->mDefault));
  };
};

Attribute & Struct::operator[](const std::string & name) {
  return mAttrs[mpMetaStruct->mIndex.find(name)->second];
};

