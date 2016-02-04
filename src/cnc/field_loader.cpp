#include "cnc/stdafx.h"
#include "cnc/field_loader.h"
#include "cnc/string_utils.h"
#include "cnc/size.h"
#include "cnc/rectangle.h"
#include "cnc/color.h"
#include "cnc/hsl_color.h"

namespace cnc {

std::unordered_map<std::type_index, std::vector<FieldLoadInfo>> FieldLoader::type_load_info_;

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

FieldLoader::MissingFieldsException::MissingFieldsException(const Message& msg, std::vector<std::string>&& missing)
  : Error(MessageBuilder(StringUtils::Join(missing, ", "), msg.file_name, msg.line_no, msg.function_name)),
  missing_(std::move(missing)) {
}

bool FieldLoader::TryGetValueFromYaml(const std::string& yaml_name, const MiniYamlMap& mm, std::string& value) {
  auto iter = mm.find(yaml_name);
  if (iter == mm.end()) {
    return false;
  }

  const auto& yaml = iter->second;
  value = StringUtils::Trim(yaml.value());
  return true;
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

  std::string str = s;
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
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