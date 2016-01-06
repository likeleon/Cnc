#include "cnc/stdafx.h"
#include "cnc/sprite_font.h"
#include "cnc/sheet_builder.h"
#include "cnc/error.h"
#include "cnc/perf_timer.h"
#include "cnc/file.h"
#include "cnc/surface_ptr.h"
#include "cnc/size.h"
#include "cnc/sheet.h"
#include "cnc/graphics_util.h"

namespace cnc {

SpriteFont::SpriteFont(const std::string& name, int32_t size, SheetBuilder& builder)
  : size_(size), builder_(builder) {
  if (builder_.type() != SheetType::BGRA) {
    throw Error(MSG("The sheet builder must create BGRA sheets"));
  }

  font_buffer_ = File::OpenRead(name);
  sdl_rw_ = SDL_RWops_UniquePtr(SDL_RWFromMem(&font_buffer_[0], static_cast<int32_t>(font_buffer_.size())));
  ttf_font_ = TTF_Font_UniquePtr(TTF_OpenFontRW(sdl_rw_.get(), 0, size_));
  if (ttf_font_ == nullptr) {
    throw Error(MSG("Cannot load font from file '" + name + "': " + TTF_GetError()));
  }

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
    iter = glyphs_.emplace(c, CreateGlyph(c.first, c.second)).first;
  }
  return iter->second;
}

SpriteFont::GlyphInfo SpriteFont::CreateGlyph(char ch, const Color& color) {
  int32_t min_x = 0, max_x = 0, min_y = 0, max_y = 0, advance = 0;
  if (TTF_GlyphMetrics(ttf_font_.get(), ch, &min_x, &max_x, &min_y, &max_y, &advance) != 0) {
    throw Error(MSG("TTF_GlyphMetrics failed with ch '" + std::string(1, ch) + "': " + TTF_GetError()));
  }

  SDL_Color sdl_color{ color.r, color.g, color.b, color.a };
  auto surface = SDL_Surface_UniquePtr(TTF_RenderGlyph_Blended(ttf_font_.get(), ch, sdl_color));
  if (surface == nullptr) {
    throw Error(MSG("TTF_RenderGlyph_Blended failed with ch '" + std::string(1, ch) + "': " + TTF_GetError()));
  }

  GlyphInfo g{
    advance,
    { min_x, -min_y },
    builder_.Allocate({ max_x - min_x, max_y - min_y })
  };

  SDL_LockSurface(surface.get());
  
  auto& s = g.sprite;
  char* p = static_cast<char*>(surface->pixels);
  char* dest = &s.sheet->GetData()[0];
  auto dest_stride = s.sheet->size().width * 4;
  for (auto y = 0; y < s.size.y; ++y) {
    for (auto x = 0; x < s.size.x; ++x) {
      if (p[x] != 0) {
        auto q = dest_stride * (y + s.bounds.Top()) + 4 * (x + s.bounds.Left());
        auto pmc = GraphicsUtil::PremultiplyAlpha(Color(p[0], color));

        dest[q] = pmc.b;
        dest[q + 1] = pmc.g;
        dest[q + 2] = pmc.r;
        dest[q + 3] = pmc.a;
      }
    }
    p += surface->pitch;
  }
  
  SDL_UnlockSurface(surface.get());
  surface = nullptr;

  s.sheet->CommitBufferedData();

  return g;
}

float SpriteFont::LineWidth(const std::string& line) {
  return std::accumulate(line.begin(), line.end(),
                         0.0f,
                         [this](float sum, char c) { return sum + Glyph(c, Color::White).advance; });
}

}