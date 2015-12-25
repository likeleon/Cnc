#pragma once

#include "cnc/field_loader.h"
#include "cnc/igraphics_device.h"
#include "cnc/size.h"

namespace cnc {

class Arguments;

struct CNC_API PlayerSettings {
  std::string name = "Newbie";

  static const std::vector<FieldLoadInfo> load_info;
};

struct CNC_API GameSettings {
  std::string mod = "modchooser";
  bool lock_mouse_window = false;
  
  static const std::vector<FieldLoadInfo> load_info;
};

struct CNC_API GraphicSettings {
  std::string renderer = "Default";
  WindowMode mode = WindowMode::PseudoFullscreen;
  Size fullscreen_size = Size(0, 0);
  Size windowed_size = Size(1024, 768);
  int32_t batch_size = 8192;

  static const std::vector<FieldLoadInfo> load_info;
};

class CNC_API Settings {
public:
  Settings(const std::string& path, const Arguments& args);

  PlayerSettings& player() { return player_; }
  GameSettings& game() { return game_; }
  GraphicSettings& graphics() { return graphics_; }

private:
  std::string settings_file_;
  PlayerSettings player_;
  GameSettings game_;
  GraphicSettings graphics_;
};

}
