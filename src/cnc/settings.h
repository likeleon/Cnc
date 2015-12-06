#pragma once

#include "cnc/field_loader.h"
#include "cnc/igraphics_device.h"
#include "cnc/size.h"

namespace cnc {

class Arguments;

struct SettingsSection {
  virtual const std::vector<FieldLoadInfo>& load_info() const = 0;
};

struct PlayerSettings : public SettingsSection {
  std::string name = "Newbie";

  const std::vector<FieldLoadInfo>& load_info() const override { return load_info_; }
  static const std::vector<FieldLoadInfo> load_info_;
};

struct GraphicSettings : public SettingsSection {
  std::string renderer = "Default";
  WindowMode mode = WindowMode::PseudoFullscreen;
  Size fullscreen_size = Size(0, 0);
  Size windowed_size = Size(1024, 768);

  const std::vector<FieldLoadInfo>& load_info() const override { return load_info_; }
  static const std::vector<FieldLoadInfo> load_info_;
};

class Settings {
public:
  Settings(const std::string& path, const Arguments& args);

  PlayerSettings& player() { return player_; }
  GraphicSettings& graphics() { return graphics_; }

private:
  std::string settings_file_;
  PlayerSettings player_;
  GraphicSettings graphics_;
  std::unordered_map<std::string, SettingsSection*> sections_;
};

}
