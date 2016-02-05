#include "cnc/stdafx.h"
#include "cnc/settings.h"
#include "cnc/mini_yaml.h"
#include "cnc/file.h"
#include "cnc/field_loader.h"
#include "cnc/enum_info.h"

namespace cnc {

const std::vector<FieldLoadInfo> PlayerSettings::load_info = {
  { "Name", StringFieldInfo(&PlayerSettings::name) }
};

const std::vector<FieldLoadInfo> GameSettings::load_info = {
  { "Mod", StringFieldInfo(&GameSettings::mod) },
  { "PreviousMod", StringFieldInfo(&GameSettings::previous_mod) }
};

const std::vector<FieldLoadInfo> GraphicSettings::load_info = {
  { "Renderer", StringFieldInfo(&GraphicSettings::renderer) },
  { "Mode", EnumFieldInfo<WindowModeTraits>(&GraphicSettings::mode) },
  { "FullscreenSize", TypeFieldInfo(&GraphicSettings::fullscreen_size) },
  { "WindowedSize", TypeFieldInfo(&GraphicSettings::windowed_size) },
  { "PixelDouble", TypeFieldInfo(&GraphicSettings::pixel_double) },
  { "CursorDouble", TypeFieldInfo(&GraphicSettings::cursor_double) },
  { "BatchSize", TypeFieldInfo(&GraphicSettings::batch_size) },
  { "SheetSize", TypeFieldInfo(&GraphicSettings::sheet_size) }
};

template <typename T>
static void LoadSectionYaml(const std::string& key, T& section, const MiniYamlMap& yaml) {
  auto iter = yaml.find(key);
  if (iter != yaml.end()) {
    FieldLoader::Load(section, iter->second);
  }
}

Settings::Settings(const std::string& path, const Arguments& /*args*/)
  : settings_file_(path) {
  
  try {
    if (File::Exists(settings_file_)) {
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
