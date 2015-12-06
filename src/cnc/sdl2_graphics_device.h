#pragma once

#include "cnc/thread_affine.h"
#include "cnc/igraphics_device.h"
#include "cnc/size.h"

namespace cnc {

class Sdl2GraphicsDevice : public ThreadAffine, public IGraphicsDevice {
public:
  Sdl2GraphicsDevice(const Size& window_size, WindowMode window_mode);
  virtual ~Sdl2GraphicsDevice();

  void GrabWindowMouseFocus();
  void ReleaseWindowMouseFocus();

  const Size& window_size() const { return window_size_; }

private:
  Size window_size_;
  SDL_Window* window_ = nullptr;
  SDL_GLContext context_ = nullptr;
};

}