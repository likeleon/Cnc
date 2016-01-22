#include "cnc/stdafx.h"
#include "cnc/hardware_palette.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/igraphics_device.h"

namespace cnc {

HardwarePalette::HardwarePalette()
  : texture_(Game::renderer()->device().CreateTexture()) {
}

}