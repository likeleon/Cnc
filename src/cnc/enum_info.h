#pragma once

#include "cnc/error.h"

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
const std::string& EnumToName(E value);

template <typename E>
E NameToEnum(const std::string& name);

template <typename E>
E NameToEnum(const std::string& name, E default_value);

}

#include "enum_info.inl"