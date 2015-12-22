#pragma once

namespace cnc {

struct SdlSurfaceDeleter {
  void operator()(SDL_Surface* sdl_surface) {
    SDL_FreeSurface(sdl_surface);
  }
};
using SdlSurfacePtr = std::unique_ptr<SDL_Surface, SdlSurfaceDeleter>;

}
