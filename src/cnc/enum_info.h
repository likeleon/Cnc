#pragma once

#include "cnc/debug.h"

namespace cnc {

template <typename E>
class EnumInfo {
public:
  using NamesType = std::unordered_map<E, std::string>;

  static const std::vector<E>& enums();
  static const std::vector<std::string>& names();

private:
  static std::vector<E> enums_;
  static std::vector<std::string> names_;
};

template <typename E>
struct EnumInfoTraits;

template <typename E>
const std::vector<E>& EnumInfo<E>::enums() {
  if (enums_.empty()) {
    const auto& names = EnumInfoTraits<E>::names;
    std::transform(names.begin(), names.end(), std::back_inserter(enums_),
                   [](typename decltype(names)::const_reference val) { return val.first; });
  }
  return enums_;
}

template <typename E>
const std::vector<std::string>& EnumInfo<E>::names() {
  if (names_.empty()) {
    const auto& names = EnumInfoTraits<E>::names;
    std::transform(names.begin(), names.end(), std::back_inserter(names_),
                   [](typename decltype(names)::const_reference val) { return val.second; });
  }
  return names_;
}

template <typename E>
const std::string& EnumToName(E value) {
  const auto& it = EnumInfoTraits<E>::names.find(value);
  if (it == EnumInfoTraits<E>::names.end()) {
    Debug::Die(std::string("Missing ") + EnumInfoTraits<E>::pretty_name);
  }
  return it->second;
}

template <typename E>
E NameToEnum(const std::string& name) {
  for (const auto& kvp : EnumInfoTraits<E>::names) {
    if (kvp.second == name) {
      return kvp.first;
    }
  }
  Debug::Die(std::string("Invalid ") + EnumInfoTraits<E>::pretty_name + " name: '" + name + "'");
}

template <typename E>
E NameToEnum(const std::string& name, E default_value) {
  for (const auto& kvp : EnumInfoTraits<E>::names) {
    if (kvp.second == name) {
      return kvp.first;
    }
  }
  return default_value;
}

}