#pragma once

namespace cnc {

struct Size;
struct Rectangle;

class CNC_API Bitmap {
public:
  explicit Bitmap(const std::string& filename);
  explicit Bitmap(StreamPtr stream);
  explicit Bitmap(SDL_Surface_UniquePtr surface);

  Bitmap(const Bitmap&) = delete;
  Bitmap& operator=(const Bitmap&) = delete;

  Size Size() const;
  Rectangle Bounds() const;

  SDL_Surface& surface() const;

private:
  SDL_Surface_UniquePtr surface_;
};

using BitmapUniquePtr = std::unique_ptr<Bitmap>;

}
