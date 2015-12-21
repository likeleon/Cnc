#pragma once

#include "cnc/igraphics_device.h"

namespace cnc {

struct GraphicSettings;
class IInputHandler;

class CNC_API Renderer {
public:
  explicit Renderer(const GraphicSettings& graphic_settings);

  void BeginFrame();
  void EndFrame(IInputHandler& input_handler);

  const Size& Resolution() const;

private:
  GraphicsDevicePtr device_;
};

}
