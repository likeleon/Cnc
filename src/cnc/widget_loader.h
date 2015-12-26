#pragma once

#include "cnc/mini_yaml.h"

namespace cnc {

class ModData;

class WidgetLoader {
public:
  explicit WidgetLoader(const ModData& mod_data);

private:
  const ModData& mod_data_;
  std::unordered_map<std::string, MiniYamlNode> widgets_;
};

}