#include "cnc/stdafx.h"
#include "cnc/field_loader.h"

namespace cnc {

std::unordered_map<std::type_index, std::vector<FieldLoadInfo>> FieldLoader::type_load_info_;

FieldLoader::MissingFieldsException::MissingFieldsException(const Message& msg, std::vector<std::string>&& missing)
  : YamlException(MessageBuilder(StringUtils::Join(missing, ", "), msg.file_name, msg.line_no, msg.function_name)),
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

std::vector<std::string> FieldLoader::GetValueStringVector(const MiniYaml& yaml) {
  std::vector<std::string> ret;
  auto parts = StringUtils::Split(yaml.value(), ',', StringSplitOptions::RemoveEmptyEntries);
  for (const auto& p : parts) {
    ret.emplace_back(StringUtils::Trim(p));
  }
  return ret;
}

}