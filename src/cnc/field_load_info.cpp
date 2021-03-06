#include "cnc/stdafx.h"
#include "cnc/field_load_info.h"
#include "cnc/size.h"
#include "cnc/rectangle.h"
#include "cnc/color.h"
#include "cnc/hsl_color.h"
#include "cnc/error.h"

namespace cnc {

FieldInfo::FieldInfo(Setter setter)
  : setter_(setter) {
  assert(setter != nullptr);
}

void FieldInfo::SetValue(void* obj, const std::string& value) const {
  assert(obj != nullptr);
  setter_(obj, value);
}

FieldLoadInfo::FieldLoadInfo(const std::string& yaml_name,
                             const FieldInfo& field_info,
                             bool required)
  : yaml_name(std::move(yaml_name)), required(required), field_info(field_info) {
}

template <>
Size CNC_API FieldInfoTraits<Size>::Parse(const std::string& s) {
  auto parts = StringUtils::Split(s, ',', StringSplitOptions::RemoveEmptyEntries);
  return{ std::stoi(parts[0]), std::stoi(parts[1]) };
}

template <>
Rectangle CNC_API FieldInfoTraits<Rectangle>::Parse(const std::string& s) {
  auto parts = StringUtils::Split(s, ',', StringSplitOptions::RemoveEmptyEntries);
  return{
    std::stoi(parts[0]),
    std::stoi(parts[1]),
    std::stoi(parts[2]),
    std::stoi(parts[3])
  };
}

template <>
std::string CNC_API FieldInfoTraits<std::string>::Parse(const std::string& s) {
  return s;
}

template <>
Color CNC_API FieldInfoTraits<Color>::Parse(const std::string& s) {
  Color rgb = Color::Transparent;
  if (HSLColor::TryParseRGB(s, rgb)) {
    return rgb;
  }
  // TODO: InvalidValueAction(value, ...)
  throw Error(MSG("Invalid value for color: " + s));
}

template <>
int32_t CNC_API FieldInfoTraits<int32_t>::Parse(const std::string& s) {
  return std::stoi(s);
}

static bool ParseYesNo(const std::string& s) {
  if (s.empty()) {
    throw Error(MSG("Invalid value for bool: " + s));
  }

  std::string str = StringUtils::ToLower(s);
  if (str == "yes" || str == "true") {
    return true;
  }
  if (str == "no" || str == "false") {
    return false;
  }

  throw Error(MSG("Invalid value for bool: " + s));
}

template <>
bool CNC_API FieldInfoTraits<bool>::Parse(const std::string& s) {
  return ParseYesNo(s);
}

}
