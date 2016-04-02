#include "cnc/stdafx.h"
#include "cnc/type_dictionary.h"
#include "cnc/error.h"
#include "cnc/itype_exposable.h"

namespace cnc {

void TypeDictionary::Add(const TypeExposablePtr& val) {
  for (const auto& tt : val->Types()) {
    InnerAdd(tt, val);
  }
}

void TypeDictionary::InnerAdd(std::type_index t, const TypeExposablePtr& val) {
  data_.try_emplace(t).first->second.emplace_back(val);
}

void TypeDictionary::Remove(ITypeExposable& val) {
  for (const auto& tt : val.Types()) {
    InnerRemove(tt, val);
  }
}

void TypeDictionary::InnerRemove(std::type_index t, ITypeExposable& val) {
  auto iter = data_.find(t);
  if (iter == data_.end()) {
    return;
  }

  auto& c = iter->second;
  c.erase(std::remove(c.begin(), c.end(), val.shared_from_this()));
  if (c.empty()) {
    data_.erase(t);
  }
}

ITypeExposable* TypeDictionary::Get(std::type_index t, bool throws_if_missing) {
  auto iter = data_.find(t);
  if (iter == data_.end()) {
    if (throws_if_missing) {
      throw Error(MSG("TypeDictionary does not contain instance of type '" + std::string(t.name()) + "'"));
    }
    return nullptr;
  }

  const auto& c = iter->second;
  if (c.size() > 1) {
    throw Error(MSG("TypeDictionary contains multiple instances of type '" + std::string(t.name()) + "'"));
  }
  return c.front().get();
}

}
