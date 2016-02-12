#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/enum_info.h"
#include "cnc/error.h"
#include "cnc/string_utils.h"
#include "cnc/field_load_info.h"

namespace cnc {

class CNC_API FieldLoader {
public:
  class MissingFieldsException : public Error {
  public:
    explicit MissingFieldsException(const Message& msg, std::vector<std::string>&& missing);

    const std::vector<std::string>& missing() const { return missing_; }

  private:
    std::vector<std::string> missing_;
  };

  template <typename T>
  static T Load(const MiniYaml& my) {
    T t;
    Load(t, my);
    return t;
  }

  template <typename T>
  static void Load(T& obj, const MiniYaml& my) {
    auto load_info = RetrieveLoadInfo<T>();
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

    if (!missing.empty()) {
      throw MissingFieldsException(MSG(""), std::move(missing));
    }
  }

  template <typename T>
  static void LoadField(T& obj, const FieldInfo& fi, const std::string& value) {
    fi.SetValue(&obj, value);
  }

  template <typename T>
  static void LoadField(T& obj, const std::string& name, const std::string& value) {
    const FieldInfo* fi = obj.GetFieldInfo(name);
    if (fi == nullptr) {
      throw MissingFieldsException(MSG(""), { name });
    }
    LoadField(obj, *fi, value);
  }

  template <typename T>
  static T GetValue(const std::string& value) {
    return GetValue<T>(MiniYaml(value));
  }

  template <typename T>
  static T GetValue(const MiniYaml& yaml) {
    auto value = StringUtils::Trim(yaml.value());
    return FieldInfoTraits<T>::Parse(value);
  }

  template <typename TEnumTraits>
  static typename TEnumTraits::E GetEnumValue(const std::string& value) {
    return GetEnumValue<TEnumTraits>(MiniYaml(value));
  }

  template <typename TEnumTraits>
  static typename TEnumTraits::E GetEnumValue(const MiniYaml& yaml) {
    auto value = StringUtils::Trim(yaml.value());
    return NameToEnum<TEnumTraits>(value, true);
  }

  static std::vector<std::string> GetValueStringVector(const MiniYaml& yaml);

private:
  static bool TryGetValueFromYaml(const std::string& yaml_name, const MiniYamlMap& mm, std::string& value);

  template <typename T>
  static const std::vector<FieldLoadInfo>& RetrieveLoadInfo() {
    auto it = type_load_info_.find(typeid(T));
    if (it == type_load_info_.end()) {
      it = type_load_info_.emplace(typeid(T), T::load_info).first;
    }
    return it->second;
  }

  static std::unordered_map<std::type_index, std::vector<FieldLoadInfo>> type_load_info_;
};

}