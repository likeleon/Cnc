#include "cnc/stdafx.h"
#include "cnc/mod_metadata.h"
#include "cnc/platform.h"
#include "cnc/mini_yaml.h"

namespace cnc {

const std::vector<FieldLoadInfo> ModMetadata::load_info = {
  { "Title", StringFieldInfo<ModMetadata>(&ModMetadata::title) },
  { "Description", StringFieldInfo<ModMetadata>(&ModMetadata::description) },
  { "Version", StringFieldInfo<ModMetadata>(&ModMetadata::version) },
  { "Author", StringFieldInfo<ModMetadata>(&ModMetadata::author) },
  { "Hidden", BoolFieldInfo<ModMetadata>(&ModMetadata::hidden) }
};

std::unordered_map<std::string, ModMetadata> ModMetadata::all_mods;
bool ModMetadata::all_mods_found = false;

static std::unordered_map<std::string, ModMetadata> ValidateMods() {
  auto base_path = Platform::ResolvePaths({ std::string("."), "mods" });
  
  std::unordered_map<std::string, ModMetadata> ret;
  for (const auto& dir : Platform::GetDirectories(base_path)) {
    auto m = dir.substr(base_path.length() + 1);
    try {
      auto yaml_path = Platform::ResolvePaths({ ".", "mods", m, "mod.yaml" });
      if (!Platform::Exists(yaml_path)) {
        continue;
      }

      MiniYaml yaml("", MiniYaml::FromFile(yaml_path));
      auto nd = yaml.ToMap();
      if (nd.find("Metadata") == nd.end()) {
        continue;
      }

      auto mod = FieldLoader::Load<ModMetadata>(nd.at("Metadata"));
      mod.id = m;

      ret.emplace(m, mod);
    } catch (const std::exception& ex) {
      std::cout << "An exception occured when trying to load ModMetadata for '" + m << "'" << std::endl;
      std::cout << ex.what() << std::endl;
    }
  }
  return ret;
}

const std::unordered_map<std::string, ModMetadata>& ModMetadata::AllMods() {
  if (!all_mods_found) {
    all_mods = ValidateMods();
    all_mods_found = true;
  }
  return all_mods;
}

}
