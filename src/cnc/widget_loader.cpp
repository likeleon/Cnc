#include "cnc/stdafx.h"
#include "cnc/widget_loader.h"
#include "cnc/widget.h"
#include "cnc/widget_args.h"
#include "cnc/mod_data.h"
#include "cnc/mini_yaml.h"
#include "cnc/string_utils.h"
#include "cnc/field_loader.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/object_creator.h"

namespace cnc {

WidgetLoader::WidgetLoader(const ModData& mod_data)
  : mod_data_(mod_data) {
  auto files = MiniYaml::FromFiles(mod_data_.manifest().chrome_layout());
  for (const auto& file : files) {
    for (const auto& w : file) {
      auto key = w.key().substr(w.key().find('@') + 1);
      if (widgets_.find(key) != widgets_.end()) {
        std::ostringstream oss;
        oss << "Widget has duplicate key '" << w.key() << "' at " << w.location().ToString();
        throw Error(MSG(oss.str()));
      }
      widgets_.emplace(key, w);
    }
  }
}

WidgetPtr WidgetLoader::LoadWidget(const WidgetArgs& args, const WidgetPtr& parent, const std::string& w) const {
  auto kvp = widgets_.find(w);
  if (kvp == widgets_.end()) {
    throw Error(MSG("Cannot find widget with id'" + w + "'"));
  }

  return LoadWidget(args, parent, kvp->second);
}

WidgetPtr WidgetLoader::LoadWidget(const WidgetArgs& a, const WidgetPtr& parent, const MiniYamlNode& node) const {
  WidgetArgs args(a);

  auto widget = NewWidget(node.key(), args);

  if (parent != nullptr) {
    parent->AddChild(widget);
  }

  if (node.key().find('@') != std::string::npos) {
    FieldLoader::LoadField(*widget, TypeFieldInfo(&Widget::id_), StringUtils::Split(node.key(), '@')[1]);
  }

  for (const auto& child : node.value().nodes()) {
    if (child.key() != "Children") {
      FieldLoader::LoadField(*widget.get(), child.key(), child.value().value());
    }
  }

  widget->Initialize(args);

  for (const auto& child : node.value().nodes()) {
    if (child.key() == "Children") {
      for (const auto& c : child.value().nodes()) {
        LoadWidget(args, widget, c);
      }
    }
  }

  auto logic_node = std::find_if(
    node.value().nodes().begin(),
    node.value().nodes().end(),
    [](const auto& n) { return n.key() == "Logic"; });
  MiniYamlMap logic;
  if (logic_node != node.value().nodes().end()) {
    logic = logic_node->value().ToMap();
  }
  args.Add("logicArgs", std::move(logic));

  widget->PostInit(args);

  args.Remove("logicArgs");

  return widget;
}

WidgetPtr WidgetLoader::NewWidget(const std::string& widget_type, const WidgetArgs& args) const {
  auto type_name = StringUtils::Split(widget_type, '@')[0];
  return Game::mod_data()->object_creator().CreateObject<Widget>(type_name + "Widget", args.args());
}

}