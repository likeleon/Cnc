#pragma once

#include "cnc/mini_yaml.h"

namespace cnc {

class FieldInfo {
public:
  using Setter = std::function<void(void* obj, const std::string& value)>;

  explicit FieldInfo(Setter setter);

  void SetValue(void* obj, const std::string& value) const;

private:
  Setter setter_;
};

template <typename TObject, typename TField>
FieldInfo StringFieldInfo(TField field) {
  return FieldInfo([field](void *o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    (obj->*field) = v;
  });
}

struct FieldLoadInfo {
  FieldLoadInfo(const std::string& yaml_name, const FieldInfo& field_info);

  std::string yaml_name;
  FieldInfo field_info;
};

class FieldLoader {
public:
  template <typename T>
  static void Load(T& obj, const MiniYaml& my, const std::vector<FieldLoadInfo>& load_info) {
    if (load_info.empty()) {
      return;
    }

    auto mm = my.ToMap();
    for (const auto& fli : load_info) {
      std::string value;
      if (!TryGetValueFromYaml(fli.yaml_name, mm, value)) {
        // TODO: throw MissingFieldsException
      }
      fli.field_info.SetValue(&obj, value);
    }
  }

private:
  static bool TryGetValueFromYaml(const std::string& yaml_name, 
                                  const std::unordered_map<std::string, MiniYaml>& mm, 
                                  std::string& value);
};

}