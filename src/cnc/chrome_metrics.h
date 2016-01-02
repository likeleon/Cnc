#pragma once

#include "cnc/field_loader.h"

namespace cnc {

class CNC_API ChromeMetrics {
public:
  static void Initialize(const std::vector<std::string>& yaml);
  
  ChromeMetrics() = delete;

  template <typename T>
  static T Get(const std::string& key) {
    return FieldLoader::GetValue<T>(key, data_[key]);
  }

private:
  static std::map<std::string, std::string> data_;
};

}
