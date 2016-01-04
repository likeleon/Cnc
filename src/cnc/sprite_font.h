#pragma once

#include "cnc/point.h"
#include "cnc/Sprite.h"
#include "cnc/color.h"
#include "cnc/pair_hash.h"

namespace cnc {

class SheetBuilder;

class SpriteFont {
public:
  SpriteFont(const std::string& name, int32_t size, SheetBuilder& builder);

private:
  struct GlyphInfo {
    float advance;
    Point offset;
    //Sprite sprite;
  };

  void PrecacheColor(const Color& color, const std::string& color_name, const std::string& name);
  GlyphInfo& Glyph(char character, const Color& color);
  GlyphInfo CreateGlyph(const std::pair<char, Color>& c);
  float LineWidth(const std::string& line);

  int32_t size_;
  SheetBuilder& builder_;
  std::unordered_map<std::pair<char, Color>, GlyphInfo, PairHash> glyphs_;
};

using SpriteFontPtr = std::unique_ptr<SpriteFont>;

}
