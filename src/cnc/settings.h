#pragma once

#include "cnc/field_loader.h"

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
