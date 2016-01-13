#include "cnc/stdafx.h"
#include "cnc/manifest.h"
#include "cnc/platform.h"
#include "cnc/directory.h"
#include "cnc/string.h"
#include "cnc/log.h"
#include "cnc/mini_yaml.h"
#include "cnc/file.h"

namespace cnc {

static std::unordered_map<std::string, Manifest> LoadMods() {
  auto base_path = Platform::ResolvePaths({ ".", "mods" });
  auto mods = Directory::GetDirectories(base_path);
  std::transform(mods.begin(), mods.end(), mods.begin(), [base_path](const auto& s) {
    return s.substr(base_path.length() + 1);
  });

  std::unordered_map<std::string, Manifest> ret;
  for (const auto& mod : mods) {
    if (!File::Exists(Platform::ResolvePaths({ ".", "mods", mod, "mod.yaml" }))) {
      continue;
    }

    try {
      ret.emplace(mod, Manifest(mod));
    } catch (const std::exception &e) {
      Log::Write("debug", "An exception occured while trying to load mod " + mod + ":");
      Log::Write("debug", e.what());
    }
  }
  return ret;
}

const std::unordered_map<std::string, Manifest>& Manifest::AllMods() {
  static std::unordered_map<std::string, Manifest> all_mods;
  static bool all_mods_loaded = false;
  if (!all_mods_loaded) {
    all_mods = LoadMods();
    all_mods_loaded = true;
  }
  return all_mods;
}

std::vector<std::string> YamlList(const MiniYamlMap& yaml, const std::string& key, bool parse_paths = false) {
  if (yaml.find(key) == yaml.end()) {
    return{};
  }

  std::vector<std::string> vec;
  for (const auto& kv : yaml.at(key).ToMap()) {
    vec.emplace_back(kv.first);
  }
  if (parse_paths) {
    std::transform(vec.begin(), vec.end(), vec.begin(), Platform::ResolvePath);
  }
  return vec;
}

std::map<std::string, std::string> YamlMap(const MiniYamlMap& yaml, const std::string& key) {
  if (yaml.find(key) == yaml.end()) {
    return{};
  }

  std::map<std::string, std::string> map;
  for (const auto& node : yaml.at(key).nodes()) {
    if (node.key().find('@') != std::string::npos) {
      auto split = String::Split(node.key(), '@');
      map.emplace(split[0], split[1]);
    } else {
      map.emplace(node.key(), "");
    }
  }
  return map;
}

Manifest::Manifest(const std::string& mod) {
  auto path = Platform::ResolvePaths({ ".", "mods", mod, "mod.yaml" });
  yaml_ = MiniYaml("", MiniYaml::FromFile(path)).ToMap();

  mod_ = FieldLoader::Load<ModMetadata>(yaml_.at("Metadata"));
  mod_.id = mod;

  folders_ = YamlList(yaml_, "Folders", true);
  packages_ = YamlMap(yaml_, "Packages");
  cursors_ = YamlList(yaml_, "Cursors", true);
  chrome_ = YamlList(yaml_, "Chrome", true);
  assemblies_ = YamlList(yaml_, "Assemblies", true);
  chrome_layout_ = YamlList(yaml_, "ChromeLayout", true);
  chrome_metrics_ = YamlList(yaml_, "ChromeMetrics", true);

  auto iter = yaml_.find("LoadScreen");
  if (iter == yaml_.end()) {
    throw std::exception("'LoadScreen' section is not defined.");
  }

  fonts_ = yaml_.at("Fonts").ToMap<std::pair<std::string, int32_t>>([](const MiniYaml& my) {
    auto nd = my.ToMap();
    return std::make_pair(nd.at("Font").value(), std::stoi(nd.at("Size").value()));
  });

  requires_mods_ = yaml_.at("RequiresMods").ToMap<std::string>([](const auto& m) { return m.value(); });

  load_screen_ = &(iter->second);

  if (yaml_.find("SpriteFormats") != yaml_.end()) {
    sprite_formats_ = String::Split(yaml_.at("SpriteFormats").value(), ',', StringSplitOptions::RemoveEmptyEntries);
  }
}

const ModMetadata& Manifest::mod() const {
  return mod_;
}

const std::vector<std::string>& Manifest::folders() const {
  return folders_;
}

const std::vector<std::string>& Manifest::cursors() const {
  return cursors_;
}

const std::map<std::string, std::string>& Manifest::packages() const {
  return packages_;
}

const std::vector<std::string>& Manifest::chrome() const {
  return chrome_;
}

const std::vector<std::string>& Manifest::assemblies() const {
  return assemblies_;
}

const std::vector<std::string>& Manifest::chrome_layout() const {
  return chrome_layout_;
}

const std::vector<std::string>& Manifest::chrome_metrics() const {
  return chrome_metrics_;
}

const MiniYaml& Manifest::load_screen() const {
  return *load_screen_;
}

const std::map<std::string, std::string>& Manifest::requires_mods() const {
  return requires_mods_;
}

const std::map<std::string, std::pair<std::string, int32_t>>& Manifest::fonts() const {
  return fonts_;
}

const std::vector<std::string>& Manifest::sprite_formats() const {
  return sprite_formats_;
}

}
