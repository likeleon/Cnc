#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/enum_info.h"
#include "cnc/error.h"

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

template <typename TObject, typename TField>
FieldInfo StringFieldInfo(TField TObject::*field) {
  return FieldInfo([field](void *o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    (obj->*field) = v;
  });
}

template <typename TObject, typename TField>
FieldInfo EnumFieldInfo(TField TObject::*field) {
  return FieldInfo([field](void* o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    (obj->*field) = NameToEnum<TField>(v);
  });
}

template <typename TObject>
FieldInfo Int32FieldInfo(int32_t TObject::*field) {
  return FieldInfo([field](void* o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    (obj->*field) = std::stoi(v);
  });
}

template <typename TObject>
FieldInfo BoolFieldInfo(bool TObject::*field) {
  return FieldInfo([field](void* o, const std::string& v) {
    TObject* obj = static_cast<TObject*>(o);
    if (v == "true") {
      (obj->*field) = true;
    } else if (v == "false") {
      (obj->*field) = false;
    } else {
      throw std::invalid_argument("invalid bool string");
    }
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

class FieldLoader {
public:
  class MissingFieldsException : public std::exception {
  public:
    explicit MissingFieldsException(std::vector<std::string>&& missing);

    const char* what() const override;
    const std::vector<std::string>& missing() const { return missing_; }

  private:
    std::vector<std::string> missing_;
    std::string message_;
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
      throw MissingFieldsException(std::move(missing));
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
      throw MissingFieldsException({ name });
    }
    LoadField(obj, *fi, value);
  }

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