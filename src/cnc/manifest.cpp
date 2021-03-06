#include "cnc/stdafx.h"
#include "cnc/manifest.h"
#include "cnc/platform.h"
#include "cnc/string_utils.h"
#include "cnc/log.h"
#include "cnc/file.h"
#include "cnc/path.h"
#include "cnc/field_loader.h"
#include "cnc/object_creator.h"

namespace cnc {

static std::unordered_map<std::string, Manifest> LoadMods() {
  std::unordered_map<std::string, Manifest> ret;
  for (const auto& kvp : ModMetadata::CandidateModPaths()) {
    if (!File::Exists(Path::Combine({ kvp.second, "mod.yaml" }))) {
      continue;
    }

    try {
      ret.emplace(kvp.first, Manifest(kvp.first, kvp.second));
    } catch (const std::exception &e) {
      Log::Write("debug", "An exception occured while trying to load mod " + kvp.first + ":");
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

static std::vector<std::string> YamlList(const MiniYamlMap& yaml, const std::string& key, bool parse_paths = false) {
  if (yaml.find(key) == yaml.end()) {
    return{};
  }

  std::vector<std::string> vec;
  for (const auto& n : yaml.at(key).nodes()) {
    vec.emplace_back(n.key());
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
      auto split = StringUtils::Split(node.key(), '@');
      map.emplace(split[0], split[1]);
    } else {
      map.emplace(node.key(), "");
    }
  }
  return map;
}

Manifest::Manifest(const std::string& mod_id, const std::string& _mod_path) {
  std::string mod_path = _mod_path;
  if (mod_path.empty()) {
    mod_path = ModMetadata::CandidateModPaths().at(mod_id);
  }
  auto path = Path::Combine({ mod_path, "mod.yaml" });
  yaml_ = MiniYaml("", MiniYaml::FromFile(path)).ToMap();

  mod_ = FieldLoader::Load<ModMetadata>(yaml_.at("Metadata"));
  mod_.id = mod_id;

  folders_ = YamlList(yaml_, "Folders", true);
  map_folders_ = YamlMap(yaml_, "MapFolders");
  packages_ = YamlMap(yaml_, "Packages");
  rules_ = YamlList(yaml_, "Rules", true);
  cursors_ = YamlList(yaml_, "Cursors", true);
  chrome_ = YamlList(yaml_, "Chrome", true);
  assemblies_ = YamlList(yaml_, "Assemblies", true);
  chrome_layout_ = YamlList(yaml_, "ChromeLayout", true);
  tile_sets_ = YamlList(yaml_, "TileSets", true);
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
    sprite_formats_ = FieldLoader::GetValueStringVector(yaml_.at("SpriteFormats"));
  }
}

void Manifest::LoadCustomData(ObjectCreator& oc) {
  static const std::set<std::string> ReservedModuleNames = { "Metadata", "Folders", "MapFolders", "Packages", "Rules",
    "Sequences", "VoxelSequences", "Cursors", "Chrome", "Assemblies", "ChromeLayout", "Weapons",
    "Voices", "Notifications", "Music", "Translations", "TileSets", "ChromeMetrics", "Missions",
    "ServerTraits", "LoadScreen", "LobbyDefaults", "Fonts", "SupportsMapsFrom", "SoundFormats", "SpriteFormats",
    "RequiresMods" };

  for (const auto& kvp : yaml_) {
    if (ReservedModuleNames.find(kvp.first) != ReservedModuleNames.end()) {
      continue;
    }

    // TODO: 구현되면 이 예외 제거
    const auto NotImplementedModules = { "SpriteSequenceFormat" , "GameSpeeds", "MapGrid" };
    if (std::find(std::begin(NotImplementedModules), std::end(NotImplementedModules), kvp.first) != std::end(NotImplementedModules)) {
      continue;
    }

    if (!oc.TypeRegistered(kvp.first)) {
      throw Error(MSG("'" + kvp.first + "' is not a valid mod manifest entry."));
    }

    auto module = oc.CreateObject<GlobalModData>(kvp.first, { {"yaml", static_cast<const MiniYaml&>(kvp.second)} });
    modules_.Add(std::move(module));
  }
}

}
