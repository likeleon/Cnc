#pragma once

#include "cnc/field_load_info.h"
#include "cnc/igraphics_device.h"
#include "cnc/size.h"

namespace cnc {

class Arguments;

struct CNC_API PlayerSettings {
  std::vector<FieldLoadInfo> GetLoadInfo() const;

  std::string name = "Newbie";
};

struct CNC_API GameSettings {
  std::vector<FieldLoadInfo> GetLoadInfo() const;
  
  std::string mod = "modchooser";
  std::string previous_mod = "cnc";
  bool lock_mouse_window = false;
};

struct CNC_API GraphicSettings {
  std::vector<FieldLoadInfo> GetLoadInfo() const;

  std::string renderer = "Default";
  WindowMode mode = WindowMode::PseudoFullscreen;
  Size fullscreen_size = Size(0, 0);
  Size windowed_size = Size(1024, 768);
  bool pixel_double = false;
  bool cursor_double = false;
  int32_t batch_size = 8192;
  int32_t sheet_size = 2048;
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
