#pragma once

#include "cnc/mods/common/blank_load_screen.h"
#include "cnc/sheet_ptr.h"
#include "cnc/stop_watch.h"
#include "cnc/sprite.h"
#include "cnc/float2.h"

namespace cnc {

class Renderer;

namespace mods {
namespace cnc {

class CncLoadScreen : public common::BlankLoadScreen {
public:
  ~CncLoadScreen();

  void Init(const Manifest& m, const std::map<std::string, std::string>& info) override;

private:
  std::map<std::string, std::string> load_info_;
  StopWatch load_timer_;
  SheetPtr sheet_;
  Sprite border_top_, border_bottom_, border_left_, border_right_,
    corner_top_left_, corner_top_right_, corner_bottom_left_, conner_bottom_right_;
  int32_t load_tick_ = 0;
  Float2 nod_pos_, gdi_pos_, eva_pos_;
  Sprite nod_logo_, gdi_logo_, eva_logo_, bright_block_, dim_block_;
  Rectangle bounds_;
  Renderer* r_ = nullptr;
};

}
}
}
