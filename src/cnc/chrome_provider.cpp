#include "cnc/stdafx.h"
#include "cnc/chrome_provider.h"
#include "cnc/mini_yaml.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/log.h"
#include "cnc/sheet.h"
#include "cnc/sprite.h"

namespace cnc {

std::map<std::string, ChromeProvider::Collection> ChromeProvider::collections_;
std::map<std::string, SheetPtr> ChromeProvider::cached_sheets_;
std::map<std::string, std::map<std::string, Sprite>> ChromeProvider::cached_sprites_;

void ChromeProvider::Initialize(const std::vector<std::string>& chrome_files) {
  auto yy = MiniYaml::FromFiles(chrome_files);
  auto accumulator = [](const auto& a, const auto& b) { return MiniYaml::MergePartial(a, b); };
  auto partial = std::accumulate(yy.begin(), yy.end(), MiniYamlNodes(), accumulator);
 
  auto chrome = MiniYaml::ApplyRemovals(partial);

  for (const auto& c : chrome) {
    LoadCollection(c.key(), c.value());
  }
}

void ChromeProvider::Deinitialize() {
  collections_.clear();
  cached_sheets_.clear();
  cached_sprites_.clear();
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

const Sprite* ChromeProvider::GetImage(const std::string& collection_name, const std::string& image_name) {
  std::map<std::string, Sprite>* cached_collection = nullptr;
  if (cached_sprites_.find(collection_name) != cached_sprites_.end()) {
    cached_collection = &cached_sprites_[collection_name];
    if (cached_collection->find(image_name) != cached_collection->end()) {
      return &cached_collection->at(image_name);
    }
  }

  if (collections_.find(collection_name) == collections_.end()) {
    Log::Write("debug", "Could not find collection '" + collection_name + "'");
    return nullptr;
  }
  
  const Collection& collection = collections_[collection_name];
  if (collection.regions.find(image_name) == collection.regions.end()) {
    return nullptr;
  }

  const MappedImage& mi = collection.regions.at(image_name);
  SheetPtr sheet;
  if (cached_sheets_.find(mi.src()) != cached_sheets_.end()) {
    sheet = cached_sheets_[mi.src()];
  } else {
    auto stream = Game::mod_data()->mod_files().Open(mi.src());
    sheet = std::make_shared<Sheet>(SheetType::BGRA, stream);
    cached_sheets_.emplace(mi.src(), sheet);
  }

  if (cached_collection == nullptr) {
    cached_sprites_.emplace(collection_name, std::map<std::string, Sprite>());
    cached_collection = &cached_sprites_[collection_name];
  }

  cached_collection->emplace(image_name, mi.GetImage(sheet));
  return &cached_collection->at(image_name);
}

}