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
  FieldLoadInfo(const std::string& yaml_name,
                const FieldInfo& field_info,
                bool required);

  FieldInfo field_info;
  bool required;
  std::string yaml_name;
};

class FieldLoader {
public:
  class MissingFieldsException : public std::exception {
  public:
    explicit MissingFieldsException(std::vector<std::string>&& missing);

    const char* what() const override;
    const std::vector<std::string>& missing() const { return missing_; }

  private:
    std::vector<std::string> missing_;
  };

  template <typename T>
  static void Load(T& obj, const MiniYaml& my, const std::vector<FieldLoadInfo>& load_info) {
    if (load_info.empty()) {
      return;
    }

    std::vector<std::string> missing;

    auto mm = my.ToMap();
    for (const auto& fli : load_info) {
      std::string value;

      if (!TryGetValueFromYaml(fli.yaml_name, mm, value)) {
        if (fli.required) {
          missing.emplace_back(std::move(fli.yaml_name));
        }
        continue;
      }
      fli.field_info.SetValue(&obj, value);
    }

    if (!missing.empty())
      throw MissingFieldsException(std::move(missing));
  }

private:
  static bool TryGetValueFromYaml(const std::string& yaml_name,
                                  const std::unordered_map<std::string, MiniYaml>& mm,
                                  std::string& value);
};

}