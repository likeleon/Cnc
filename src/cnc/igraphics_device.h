#pragma once

#include "cnc/enum_info.h"

namespace cnc {

class IInputHandler;

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

  virtual void Clear() = 0;
  virtual void Present() = 0;
  virtual void PumpInput(IInputHandler& input_handler) = 0;
};

using GraphicsDevicePtr = std::unique_ptr<IGraphicsDevice>;

}
