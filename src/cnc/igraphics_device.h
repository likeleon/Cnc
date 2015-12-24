#pragma once

#include "cnc/enum_info.h"
#include "cnc/ishader.h"

namespace cnc {

class IInputHandler;
struct Size;

enum class WindowMode {
  Windowed,
  Fullscreen,
  PseudoFullscreen
};

enum class BlendMode {
  None,
  Alpha,
  Additive,
  Subtractive,
  Multiply,
  Multiplicated,
  DoubleMultiplicated
};

template <>
struct CNC_API EnumInfoTraits<WindowMode> {
  static const std::string pretty_name;
  static const EnumInfo<WindowMode>::NamesType names;
};

class CNC_API IGraphicsDevice {
public:
  virtual ~IGraphicsDevice() {}

  virtual IShaderPtr CreateShader(const std::string& name) = 0;

  virtual const Size& window_size() const = 0;

  virtual void Clear() = 0;
  virtual void Present() = 0;
  virtual void PumpInput(IInputHandler& input_handler) = 0;
};

using IGraphicsDevicePtr = std::unique_ptr<IGraphicsDevice>;

}
