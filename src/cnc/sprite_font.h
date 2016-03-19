#pragma once

#include "cnc/point.h"
#include "cnc/Sprite.h"
#include "cnc/color.h"
#include "cnc/pair_hash.h"
#include "cnc/cache.h"
#include "cnc/stream_ptr.h"

#ifdef DrawText
#undef DrawText
#endif

namespace cnc {

class SheetBuilder;
struct Float2;

class CNC_API SpriteFont {
public:
  SpriteFont(const std::string& name, int32_t size, SheetBuilder& builder);

  void DrawText(const std::string& text, const Float2& location, const Color& c);
  void DrawTextWithContrast(const std::string& text, const Float2& location, const Color& fg, const Color& bg, int32_t offset);
  Size Measure(const std::string& text);

private:
  struct GlyphInfo {
    int32_t advance;
    Point offset;
    Sprite sprite;
  };

  struct SDL_RWops_Deleter {
    void operator()(SDL_RWops* rw) {
      SDL_FreeRW(rw);
    }
  };
  using SDL_RWops_UniquePtr = std::unique_ptr<SDL_RWops, SDL_RWops_Deleter>;

  struct TTF_Font_Deleter {
    void operator()(TTF_Font* font) {
      TTF_CloseFont(font);
    }
  };
  using TTF_Font_UniquePtr = std::unique_ptr<TTF_Font, TTF_Font_Deleter>;

  void PrecacheColor(const Color& color, const std::string& color_name, const std::string& name);
  GlyphInfo CreateGlyph(char ch, const Color& c);
  float LineWidth(const std::string& line);

  int32_t size_;
  SheetBuilder& builder_;
  std::vector<char> font_buffer_;
  SDL_RWops_UniquePtr sdl_rw_;
  TTF_Font_UniquePtr ttf_font_;
  Cache<std::pair<char, Color>, GlyphInfo, PairHash> glyphs_;
};

}
