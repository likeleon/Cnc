#include "cnc/stdafx.h"
#include "cnc/type_dictionary.h"
#include "cnc/error.h"
#include "cnc/type_exposable.h"

namespace cnc {

void TypeDictionary::Add(const TypeExposablePtr& val) {
  for (const auto& i : val->Interfaces()) {
    InnerAdd(i, val);
  }
  for (const auto& tt : val->BaseTypes()) {
    InnerAdd(tt, val);
  }
}

void TypeDictionary::InnerAdd(std::type_index t, const TypeExposablePtr& val) {
  data_.try_emplace(t).first->second.emplace_back(val);
}

void TypeDictionary::Remove(TypeExposable& val) {
  for (const auto& i : val.Interfaces()) {
    InnerRemove(i, val);
  }
  for (const auto& tt : val.BaseTypes()) {
    InnerRemove(tt, val);
  }
}

void TypeDictionary::InnerRemove(std::type_index t, TypeExposable& val) {
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

TypeExposable* TypeDictionary::Get(std::type_index t, bool throws_if_missing) {
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
