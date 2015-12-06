#pragma once

namespace cnc {

enum class WindowMode {
  Windowed,
  Fullscreen,
  PseudoFullscreen
};

class IGraphicsDevice {
public:
  virtual ~IGraphicsDevice() {}
};

using GraphicsDevicePtr = std::unique_ptr<IGraphicsDevice>;

}
