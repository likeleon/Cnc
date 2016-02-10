#pragma once

#include "cnc/mods/common/blank_load_screen.h"
#include "cnc/sheet_ptr.h"
#include "cnc/stop_watch.h"
#include "cnc/float2.h"
#include "cnc/input_handler.h"
#include "cnc/rectangle.h"

namespace cnc {

class Renderer;
class SpriteFont;
struct Sprite;

namespace mods {
namespace cnc {

class CncLoadScreen : public common::BlankLoadScreen {
public:
  CncLoadScreen();
  ~CncLoadScreen();

  void Init(const Manifest& m, const std::map<std::string, std::string>& info) override;
  void Display() override;

private:
  using SpriteUniquePtr = std::unique_ptr<Sprite>;

  std::map<std::string, std::string> load_info_;
  StopWatch load_timer_;
  SheetPtr sheet_;
  SpriteUniquePtr border_top_, border_bottom_, border_left_, border_right_,
    corner_top_left_, corner_top_right_, corner_bottom_left_, corner_bottom_right_;
  int32_t load_tick_ = 0;
  Float2 nod_pos_ = Float2::Zero, gdi_pos_ = Float2::Zero, eva_pos_ = Float2::Zero;
  SpriteUniquePtr nod_logo_, gdi_logo_, eva_logo_, bright_block_, dim_block_;
  Rectangle bounds_;
  Renderer* r_ = nullptr;
  bool setup_ = false;
  SpriteFont* loading_font_ = nullptr;
  SpriteFont* version_font_ = nullptr;
  std::string loading_text_, version_text_;
  Float2 loading_pos_ = Float2::Zero, version_pos_ = Float2::Zero;
  NullInputHandler nih_;
};

}
}
}
