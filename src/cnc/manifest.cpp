#include "cnc/stdafx.h"
#include "cnc/manifest.h"
#include "cnc/platform.h"
#include "cnc/string.h"
#include "cnc/log.h"

namespace cnc {

static std::unordered_map<std::string, Manifest> LoadMods() {
  auto base_path = Platform::ResolvePaths({ ".", "mods" });
  auto mods = Platform::GetDirectories(base_path);
  std::transform(mods.begin(), mods.end(), mods.begin(), [base_path](const auto& s) {
    return s.substr(base_path.length() + 1);
  });

  std::unordered_map<std::string, Manifest> ret;
  for (const auto& mod : mods) {
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

Manifest::Manifest(const std::string& mod) {
  auto path = Platform::ResolvePaths({ ".", "mods", mod, "mod.yaml" });
  yaml_ = MiniYaml("", MiniYaml::FromFile(path)).ToMap();
  
  mod_ = FieldLoader::Load<ModMetadata>(yaml_.at("Metadata"));
  mod_.id = mod;

  folders_ = YamlList(yaml_, "Folders", true);
  assemblies_ = YamlList(yaml_, "Assemblies", true);

  auto iter = yaml_.find("LoadScreen");
  if (iter == yaml_.end()) {
    throw std::exception("'LoadScreen' section is not defined.");
  }
  load_screen_ = &(iter->second);
}

std::vector<std::string> Manifest::YamlList(const std::unordered_map<std::string, MiniYaml>& yaml,
                                            const std::string& key,
                                            bool parse_paths) {
  std::vector<std::string> ret;
  
  if (yaml.find(key) == yaml.end()) {
    return ret;
  }

  for (const auto& kv : yaml.at(key).ToMap()) {
    ret.emplace_back(kv.first);
  }
  if (parse_paths) {
    std::transform(ret.begin(), ret.end(), ret.begin(), Platform::ResolvePath);
  }
  return ret;
}

const std::vector<std::string>& Manifest::folders() const {
  return folders_;
}

const std::vector<std::string>& Manifest::assemblies() const {
  return assemblies_;
}

const MiniYaml& Manifest::load_screen() const {
  return *load_screen_;
}

}
