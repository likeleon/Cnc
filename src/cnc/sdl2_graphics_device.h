#pragma once

#include "cnc/thread_affine.h"
#include "cnc/igraphics_device.h"
#include "cnc/size.h"
#include "cnc/sdl2_input.h"

namespace cnc {

class Sdl2GraphicsDevice : public ThreadAffine, public IGraphicsDevice {
public:
  Sdl2GraphicsDevice(const Size& window_size, WindowMode window_mode);
  virtual ~Sdl2GraphicsDevice();
  
  std::unique_ptr<IVertexBuffer<Vertex>> CreateVertexBuffer(int32_t size) override;
  ITexturePtr CreateTexture() override;
  IShaderPtr CreateShader(const std::string& name) override;

  const Size& window_size() const override { return window_size_; }

  void DrawPrimitives(PrimitiveType pt, int32_t first_vertex, int32_t num_vertices) override;
  void Clear() override;
  void Present() override;
  void PumpInput(IInputHandler& input_handler) override;
  
  void SetBlendMode(BlendMode mode) override;

  void SetHardwareCursor(const IHardwareCursor* cursor) override;

private:
  void GrabWindowMouseFocus();
  void ReleaseWindowMouseFocus();

  Size window_size_;
  SDL_Window* window_ = nullptr;
  SDL_GLContext context_ = nullptr;
  Sdl2Input input_;
};

}