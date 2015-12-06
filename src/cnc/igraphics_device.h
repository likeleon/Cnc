#pragma once

#include "cnc/enum_info.h"

namespace cnc {

enum class WindowMode {
  Windowed,
  Fullscreen,
  PseudoFullscreen
};

template <>
struct EnumInfoTraits<WindowMode> {
  static const std::string pretty_name;
  static const EnumInfo<WindowMode>::NamesType names;
};

class IGraphicsDevice {
public:
  virtual ~IGraphicsDevice() {}
};

using GraphicsDevicePtr = std::unique_ptr<IGraphicsDevice>;

}
