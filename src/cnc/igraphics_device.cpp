#include "cnc/stdafx.h"
#include "cnc/igraphics_device.h"

namespace cnc {

const std::string EnumInfoTraits<WindowMode>::pretty_name = "WindowMode";

const EnumInfo<WindowMode>::NamesType EnumInfoTraits<WindowMode>::names = {
  { WindowMode::Windowed, "Windowed" },
  { WindowMode::Fullscreen, "Fullscreen" },
  { WindowMode::PseudoFullscreen, "PseudoFullscreen" }
};

}