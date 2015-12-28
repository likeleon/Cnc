#include "cnc/stdafx.h"
#include "cnc/field_loader.h"
#include "cnc/string.h"
#include "cnc/size.h"
#include "cnc/rectangle.h"

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

FieldLoader::MissingFieldsException::MissingFieldsException(std::vector<std::string>&& missing)
  : missing_(std::move(missing)), message_(String::Join(missing_, ", ") ) {
}

const char* FieldLoader::MissingFieldsException::what() const {
  return message_.c_str();
}

bool FieldLoader::TryGetValueFromYaml(const std::string& yaml_name, const MiniYamlMap& mm, std::string& value) {
  auto iter = mm.find(yaml_name);
  if (iter == mm.end()) {
    return false;
  }

  const auto& yaml = iter->second;
  value = String::Trim(yaml.value());
  return true;
}

template <>
Size FieldInfoTraits<Size>::Parse(const std::string& s) {
  auto parts = String::Split(s, ',', StringSplitOptions::RemoveEmptyEntries);
  return{ std::stoi(parts[0]), std::stoi(parts[1]) };
}

template <>
Rectangle FieldInfoTraits<Rectangle>::Parse(const std::string& s) {
  auto parts = String::Split(s, ',', StringSplitOptions::RemoveEmptyEntries);
  return{ 
    std::stoi(parts[0]), 
    std::stoi(parts[1]),
    std::stoi(parts[2]),
    std::stoi(parts[3])
  };
}

}