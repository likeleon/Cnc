#include "cnc/string.h"

namespace cnc {

template <typename ETraits>
const std::string& EnumToName(typename ETraits::E value) {
  const auto& it = ETraits::names.find(value);
  if (it == ETraits::names.end()) {
    throw Error(MSG(std::string("Missing ") + ETraits::pretty_name));
  }
  return it->second;
}

template <typename ETraits>
typename ETraits::E NameToEnum(const std::string& name, bool ignore_case = false) {
  for (const auto& kvp : ETraits::names) {
    if (String::Equals(kvp.second, name, ignore_case)) {
      return kvp.first;
    }
  }
  auto text = std::string("Invalid ") + ETraits::pretty_name + " name: '" + name + "'";
  throw Error(MSG(text));
}

template <typename ETraits>
typename ETraits::E NameToEnumOrDefault(const std::string& name, typename ETraits::E default_value, bool ignore_case = false) {
  for (const auto& kvp : ETraits::names) {
    if (String::Equals(kvp.second, name, ignore_case)) {
      return kvp.first;
    }
  }
  return default_value;
}
}
