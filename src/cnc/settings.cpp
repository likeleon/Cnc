#include "cnc/stdafx.h"
#include "cnc/settings.h"
#include "cnc/mini_yaml.h"
#include "cnc/platform.h"
#include "cnc/field_loader.h"
#include "cnc/enum_info.h"

namespace cnc {

const std::vector<FieldLoadInfo> PlayerSettings::load_info = {
  { "Name", StringFieldInfo(&PlayerSettings::name) }
};

const std::vector<FieldLoadInfo> GameSettings::load_info = {
  { "Mod", StringFieldInfo(&GameSettings::mod) }
};

const std::vector<FieldLoadInfo> GraphicSettings::load_info = {
  { "Renderer", StringFieldInfo(&GraphicSettings::renderer) },
  { "Mode", EnumFieldInfo(&GraphicSettings::mode) },
  { "FullscreenSize", TypeFieldInfo(&GraphicSettings::fullscreen_size) },
  { "WindowedSize", TypeFieldInfo(&GraphicSettings::windowed_size) }
};

template <typename T>
static void LoadSectionYaml(const std::string& key, 
                            T& section, 
                            const std::unordered_map<std::string, MiniYaml>& yaml) {
  auto iter = yaml.find(key);
  if (iter != yaml.end()) {
    FieldLoader::Load(section, iter->second);
  }
}

Settings::Settings(const std::string& path, const Arguments& /*args*/)
  : settings_file_(path) {
  
  try {
    if (Platform::Exists(settings_file_)) {
      auto yaml = MiniYaml::MapFromFile(settings_file_);
      LoadSectionYaml("Player", player_, yaml);
      LoadSectionYaml("Game", game_, yaml);
      LoadSectionYaml("Graphics", graphics_, yaml);
    }

    // TODO: Override with commandline args
  } catch (...) {
  }
}

}
