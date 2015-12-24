#pragma once

#include "cnc/igraphics_device.h"
#include "cnc/sprite_renderer.h"

namespace cnc {

struct GraphicSettings;
class IInputHandler;

class CNC_API Renderer {
public:
  explicit Renderer(const GraphicSettings& graphic_settings);

  void BeginFrame();
  void EndFrame(IInputHandler& input_handler);

  const Size& Resolution() const;
  SpriteRenderer& rgba_sprite_renderer();

private:
  IGraphicsDevicePtr device_;
  std::unique_ptr<SpriteRenderer> rgba_sprite_renderer_;
};

}
