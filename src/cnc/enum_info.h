#pragma once

#include "cnc/error.h"

namespace cnc {

template <typename ETraits>
using EnumNamesType = std::unordered_map<typename ETraits::E, std::string>;

template <typename ETraits>
const std::string& EnumToName(typename ETraits::E value);

template <typename ETraits>
typename ETraits::E NameToEnum(const std::string& name);

template <typename ETraits>
typename ETraits::E NameToEnumOrDefault(const std::string& name, typename ETraits::E default_value);

}

#include "enum_info.inl"