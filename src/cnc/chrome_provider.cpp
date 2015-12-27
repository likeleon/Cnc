#include "cnc/stdafx.h"
#include "cnc/chrome_provider.h"
#include "cnc/mini_yaml.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"

namespace cnc {

std::map<std::string, ChromeProvider::Collection> ChromeProvider::collections_;

void ChromeProvider::Initialize(const std::vector<std::string>& chrome_files) {
  std::vector<MiniYamlNodes> yy;
  for (const auto& f : chrome_files) {
    yy.emplace_back(*MiniYaml::FromFile(f));
  }
  MiniYamlNodes(*accumulator)(const MiniYamlNodes&, const MiniYamlNodes&) = &MiniYaml::MergePartial;
  auto partial = std::accumulate(yy.begin(), yy.end(), MiniYamlNodes(), accumulator);
 
  auto chrome = MiniYaml::ApplyRemovals(partial);

  for (const auto& c : chrome) {
    LoadCollection(c.key(), c.value());
  }
}

void ChromeProvider::Deinitialize() {
  collections_.clear();
}

void ChromeProvider::LoadCollection(const std::string& name, const MiniYaml& yaml) {
  if (Game::mod_data()->load_screen() != nullptr) {
    Game::mod_data()->load_screen()->Display();
  }

  Collection collection;
  collection.src = yaml.value();
  for (const auto& n : yaml.nodes()) {
    collection.regions.emplace(n.key(), MappedImage(yaml.value(), n.value()));
  }

  collections_.emplace(name, collection);
}

}