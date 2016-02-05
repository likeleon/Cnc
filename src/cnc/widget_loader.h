#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/widget_ptr.h"

namespace cnc {

class ModData;
class WidgetArgs;

class WidgetLoader {
public:
  explicit WidgetLoader(const ModData& mod_data);
  ~WidgetLoader() = default;

  WidgetLoader(const WidgetLoader&) = delete;
  WidgetLoader& operator=(const WidgetLoader&) = delete;

  WidgetPtr LoadWidget(const WidgetArgs& args, const WidgetPtr& parent, const std::string& w) const;
  WidgetPtr LoadWidget(const WidgetArgs& args, const WidgetPtr& parent, const MiniYamlNode& node) const;

private:
  WidgetPtr NewWidget(const std::string& widget_type, const WidgetArgs& args) const;

  const ModData& mod_data_;
  std::unordered_map<std::string, MiniYamlNode> widgets_;
};

}