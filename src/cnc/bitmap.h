#pragma once

#include "cnc/surface_ptr.h"

namespace cnc {

struct Size;
struct Rectangle;

class Bitmap {
public:
  explicit Bitmap(const std::string& filename);
  explicit Bitmap(const std::vector<char>& stream);
  explicit Bitmap(SDL_Surface_UniquePtr surface);

  Bitmap(const Bitmap&) = delete;
  Bitmap& operator=(const Bitmap&) = delete;

  Size Size() const;
  Rectangle Bounds() const;

  SDL_Surface& surface();

private:
  SDL_Surface_UniquePtr surface_;
};

using BitmapUniquePtr = std::unique_ptr<Bitmap>;

}
