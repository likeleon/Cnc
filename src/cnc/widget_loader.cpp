#include "cnc/stdafx.h"
#include "cnc/widget_loader.h"
#include "cnc/mod_data.h"
#include "cnc/mini_yaml.h"
#include "cnc/string.h"

namespace cnc {

WidgetLoader::WidgetLoader(const ModData& mod_data)
  : mod_data_(mod_data) {
  std::vector<MiniYamlNodesPtr> files;
  for (const auto& c : mod_data_.manifest().chrome_layout()) {
    files.emplace_back(MiniYaml::FromFile(c));
  }
  for (const auto& file : files) {
    for (const auto& w : *file) {
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

}