#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/enum_info.h"
#include "cnc/error.h"
#include "cnc/string_utils.h"

namespace cnc {

class CNC_API FieldInfo {
public:
  using Setter = std::function<void(void* obj, const std::string& value)>;

  explicit FieldInfo(Setter setter);

  void SetValue(void* obj, const std::string& value) const;

private:
  Setter setter_;
};

template <typename T>
struct FieldInfoTraits {
  static T Parse(const std::string& s);
};

template <typename TObject>
FieldInfo StringVectorFieldInfo(std::vector<std::string> TObject::*field) {
  return FieldInfo([field](void *o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    auto parts = StringUtils::Split(v, ',', StringSplitOptions::RemoveEmptyEntries);
    for (const auto& part : parts) {
      (obj->*field).emplace_back(StringUtils::Trim(part));
    }
  });
}

template <typename TEnumTraits, typename TObject>
FieldInfo EnumFieldInfo(typename TEnumTraits::E TObject::*field) {
  return FieldInfo([field](void* o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    (obj->*field) = NameToEnum<TEnumTraits>(v, true);
  });
}

template <typename TObject, typename TField>
FieldInfo TypeFieldInfo(TField TObject::*field) {
  return FieldInfo([field](void* o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    (obj->*field) = FieldInfoTraits<TField>::Parse(v);
  });
}

struct FieldLoadInfo {
  FieldLoadInfo(const std::string& yaml_name,
                const FieldInfo& field_info,
                bool required = false);

  FieldInfo field_info;
  bool required;
  std::string yaml_name;
};

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