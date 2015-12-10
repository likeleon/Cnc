#pragma once

#include "cnc/igraphics_device.h"

namespace cnc {

struct GraphicSettings;
class IInputHandler;

class Renderer {
public:
  explicit Renderer(const GraphicSettings& graphic_settings);

  void BeginFrame();
  void EndFrame(IInputHandler& input_handler);

private:
  GraphicsDevicePtr device_;
};

}
