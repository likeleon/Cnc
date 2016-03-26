#include "cnc/stdafx.h"
#include "cnc/mod_metadata.h"
#include "cnc/platform.h"
#include "cnc/directory.h"
#include "cnc/file.h"
#include "cnc/path.h"
#include "cnc/mini_yaml.h"
#include "cnc/field_loader.h"

namespace cnc {

std::vector<FieldLoadInfo> ModMetadata::GetLoadInfo() const {
  return{
    { "Title", TypeFieldInfo(&ModMetadata::title) },
    { "Description", TypeFieldInfo(&ModMetadata::description) },
    { "Version", TypeFieldInfo(&ModMetadata::version) },
    { "Author", TypeFieldInfo(&ModMetadata::author) },
    { "Hidden", TypeFieldInfo(&ModMetadata::hidden) }
  };
};

static std::map<std::string, std::string> candidate_mod_paths_;
static std::map<std::string, ModMetadata> all_mods_;

static std::map<std::string, std::string> GetCandidateMods() {
  std::map<std::string, std::string> mods;

  auto base_path = Platform::ResolvePath(Path::Combine({ ".", "mods" }));
  for (const auto& d : Directory::GetDirectories(base_path)) {
    mods.emplace(d.substr(base_path.length() + 1), d);
  }

  auto support_path = Platform::ResolvePath(Path::Combine({ "^", "mods" }));
  if (!Directory::Exists(support_path)) {
    return mods;
  }
  for (const auto& d : Directory::GetDirectories(support_path)) {
    mods.emplace(d.substr(support_path.length() + 1), d);
  }

  return mods;
}

const std::map<std::string, std::string>& ModMetadata::CandidateModPaths() {
  static bool done = false;
  if (!done) {
    candidate_mod_paths_ = GetCandidateMods();
    done = true;
  }
  return candidate_mod_paths_;
}

static std::map<std::string, ModMetadata> ValidateMods() {
  std::map<std::string, ModMetadata> ret;
  for (const auto& kvp : ModMetadata::CandidateModPaths()) {
    try {
      auto yaml_path = Path::Combine({ kvp.second, "mod.yaml" });
      if (!File::Exists(yaml_path)) {
        continue;
      }

      MiniYaml yaml("", MiniYaml::FromFile(yaml_path));
      auto nd = yaml.ToMap();
      if (nd.find("Metadata") == nd.end()) {
        continue;
      }

      auto metadata = FieldLoader::Load<ModMetadata>(nd.at("Metadata"));
      metadata.id = kvp.first;

      if (nd.find("ContentInstaller") != nd.end()) {
        metadata.content = FieldLoader::Load<ContentInstaller>(nd.at("ContentInstaller"));
      }

      ret.emplace(kvp.first, metadata);
    } catch (const std::exception& ex) {
      std::cout << "An exception occured when trying to load ModMetadata for '" + kvp.first << "'" << std::endl;
      std::cout << ex.what() << std::endl;
    }
  }
  return ret;
}

const std::map<std::string, ModMetadata>& ModMetadata::AllMods() {
  static bool all_mods_found = false;
  if (!all_mods_found) {
    all_mods_ = ValidateMods();
    all_mods_found = true;
  }
  return all_mods_;
}

}
