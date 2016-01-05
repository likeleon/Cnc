#include "cnc/stdafx.h"
#include "cnc/sprite_font.h"
#include "cnc/sheet_builder.h"
#include "cnc/error.h"
#include "cnc/perf_timer.h"
#include "cnc/file.h"
#include "cnc/surface_ptr.h"
#include "cnc/size.h"

namespace cnc {

SpriteFont::SpriteFont(const std::string& name, int32_t size, SheetBuilder& builder)
  : size_(size), builder_(builder) {
  if (builder_.type() != SheetType::BGRA) {
    throw Error(MSG("The sheet builder must create BGRA sheets"));
  }

  font_buffer_ = File::OpenRead(name);
  sdl_rw_ = SDL_RWops_UniquePtr(SDL_RWFromMem(&font_buffer_[0], static_cast<int32_t>(font_buffer_.size())));
  ttf_font_ = TTF_Font_UniquePtr(TTF_OpenFontRW(sdl_rw_.get(), 0, size_));

  PrecacheColor(Color::White, "White", name);
  PrecacheColor(Color::Red, "Red", name);
}

static std::string PerfTimerName(const std::string& name,
                                 int32_t size,
                                 const std::string& color_name) {
  std::ostringstream oss;
  oss << "PrecacheColor " << name << " " << size << "px " << color_name;
  return oss.str();
}

void SpriteFont::PrecacheColor(const Color& color, const std::string& color_name, const std::string& name) {
  PERF_TIMER(PerfTimerName(name, size_, color_name), {
    for (char c = 0x20; c < 0x7f; ++c) {
      Glyph(c, color);
    }
  });
}

SpriteFont::GlyphInfo& SpriteFont::Glyph(char character, const Color& color) {
  auto c = std::make_pair(character, color);
  auto iter = glyphs_.find(c);
  if (iter == glyphs_.end()) {
    iter = glyphs_.emplace(c, CreateGlyph(c)).first;
  }
  return iter->second;
}

SpriteFont::GlyphInfo SpriteFont::CreateGlyph(const std::pair<char, Color>& c) {
  char ch = c.first;

  int32_t min_x = 0, max_x = 0, min_y = 0, max_y = 0, advance = 0;
  TTF_GlyphMetrics(ttf_font_.get(), ch, &min_x, &max_x, &min_y, &max_y, &advance);

  /*SDL_Color color{ c.second.r, c.second.g, c.second.b, c.second.a };
  auto surface = SDL_Surface_UniquePtr(TTF_RenderGlyph_Blended(ttf_font_.get(), ch, color));*/

  GlyphInfo g{
    advance,
    { min_x, -min_y },
    builder_.Allocate({ max_x - min_x, max_y - min_y })
  };

  return g;
}

float SpriteFont::LineWidth(const std::string& line) {
  return std::accumulate(line.begin(), line.end(),
                         0.0f,
                         [this](float sum, char c) { return sum + Glyph(c, Color::White).advance; });
}

}