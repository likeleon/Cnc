#include "cnc/stdafx.h"
#include "cnc/field_loader.h"
#include "cnc/string.h"

namespace cnc {

FieldInfo::FieldInfo(Setter setter)
  : setter_(setter) {
  assert(setter != nullptr);
}

void FieldInfo::SetValue(void* obj, const std::string& value) const {
  assert(obj != nullptr);
  setter_(obj, value);
}

FieldLoadInfo::FieldLoadInfo(const std::string& yaml_name, const FieldInfo& field_info)
  : yaml_name(std::move(yaml_name)), field_info(field_info) {
}

bool FieldLoader::TryGetValueFromYaml(const std::string& yaml_name,
                                      const std::unordered_map<std::string, MiniYaml>& mm,
                                      std::string& value) {
  auto iter = mm.find(yaml_name);
  if (iter == mm.end()) {
    return false;
  }

  const auto& yaml = iter->second;
  value = Trim(yaml.value());
  return true;
}

}