#include "cnc/stdafx.h"
#include "cnc/device_factory.h"
#include "cnc/sdl2_graphics_device.h"

namespace cnc {

IGraphicsDevicePtr DeviceFactory::CreateGraphics(const Size& size, WindowMode window_mode) {
  return std::make_unique<Sdl2GraphicsDevice>(size, window_mode);
}

}