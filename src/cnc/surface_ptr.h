#pragma once

namespace cnc {

struct SDL_Surface_Deleter {
  void operator()(SDL_Surface* sdl_surface) {
    SDL_FreeSurface(sdl_surface);
  }
};
using SDL_Surface_UniquePtr = std::unique_ptr<SDL_Surface, SDL_Surface_Deleter>;

}
