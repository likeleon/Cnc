#pragma once

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

struct CNC_API FieldLoadInfo {
  FieldLoadInfo(const std::string& yaml_name, const FieldInfo& field_info, bool required = false);

  FieldInfo field_info;
  bool required;
  std::string yaml_name;
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

}
