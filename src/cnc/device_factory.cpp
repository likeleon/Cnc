#include "cnc/stdafx.h"
#include "cnc/device_factory.h"

namespace cnc {

GraphicsDevicePtr DeviceFactory::CreateGraphics(const Size& /*size*/, WindowMode /*window_mode*/) {
  // TODO
  return nullptr;
}

}