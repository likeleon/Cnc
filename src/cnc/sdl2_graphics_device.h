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

  const Size& window_size() const { return window_size_; }

  void Clear() override;
  void Present() override;
  void PumpInput(IInputHandler& input_handler) override;

private:
  void GrabWindowMouseFocus();
  void ReleaseWindowMouseFocus();

  Size window_size_;
  SDL_Window* window_ = nullptr;
  SDL_GLContext context_ = nullptr;
  Sdl2Input input_;
};

}