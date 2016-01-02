#include "cnc/stdafx.h"
#include "cnc/igraphics_device.h"

namespace cnc {

const std::string WindowModeTraits::pretty_name = "WindowMode";

const EnumNamesType<WindowModeTraits> WindowModeTraits::names = {
  { WindowMode::Windowed, "Windowed" },
  { WindowMode::Fullscreen, "Fullscreen" },
  { WindowMode::PseudoFullscreen, "PseudoFullscreen" }
};

}