#pragma once

#include "cnc/mini_yaml.h"
#include "cnc/widget.h"

namespace cnc {

class ModData;

class WidgetLoader {
public:
  explicit WidgetLoader(const ModData& mod_data);

  WidgetPtr LoadWidget(const WidgetArgs& args, const WidgetPtr& parent, const std::string& w) const;
  WidgetPtr LoadWidget(const WidgetArgs& args, const WidgetPtr& parent, const MiniYamlNode& node) const;

private:
  WidgetPtr NewWidget(const std::string& widget_type, const WidgetArgs& args) const;

  const ModData& mod_data_;
  std::unordered_map<std::string, MiniYamlNode> widgets_;
};

}