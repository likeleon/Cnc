#pragma once

#include "cnc/igraphics_device.h"

namespace cnc {

struct Size;
enum class WindowMode;

class DeviceFactory {
public:
  static IGraphicsDevicePtr CreateGraphics(const Size& size, WindowMode window_mode);
};
}