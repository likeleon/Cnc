#pragma once

#include "cnc/igraphics_device.h"

namespace cnc {

struct GraphicSettings;

class Renderer {
public:
  explicit Renderer(const GraphicSettings& graphic_settings);

  void BeginFrame();
  void EndFrame();

private:
  GraphicsDevicePtr device_;
};

}
