#include "cnc/stdafx.h"
#include "cnc/sprite_font.h"
#include "cnc/sheet_builder.h"
#include "cnc/error.h"
#include "cnc/perf_timer.h"
#include "cnc/file.h"
#include "cnc/size.h"
#include "cnc/sheet.h"
#include "cnc/graphics_util.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/sprite_renderer.h"
#include "cnc/stream.h"

namespace cnc {

SpriteFont::SpriteFont(const std::string& name, int32_t size, SheetBuilder& builder)
  : size_(size), builder_(builder),
  glyphs_([this](const auto& c) { return CreateGlyph(c.first, c.second); }) {
  if (builder_.type() != SheetType::BGRA) {
    throw Error(MSG("The sheet builder must create BGRA sheets"));
  }

  auto font_stream = File::OpenRead(name);
  font_buffer_ = font_stream->ReadBytes(font_stream->Length());
  sdl_rw_ = SDL_RWops_UniquePtr(SDL_RWFromMem(&font_buffer_[0], static_cast<int32_t>(font_buffer_.size())));
  ttf_font_ = TTF_Font_UniquePtr(TTF_OpenFontRW(sdl_rw_.get(), 0, size_));
  if (ttf_font_ == nullptr) {
    throw Error(MSG("Cannot load font from file '" + name + "': " + TTF_GetError()));
  }

  PrecacheColor(Color::White, "White", name);
  PrecacheColor(Color::Red, "Red", name);
}

void SpriteFont::DrawText(const std::string& text, const Float2& loc, const Color& color) {
  Float2 location{ loc.x, loc.y + size_ };
  auto p = location;
  for (char c : text) {
    if (c == '\n') {
      location.y += size_;
      p = location;
      continue;
    }

    const auto& g = glyphs_[std::make_pair(c, color)];
    Float2 draw_loc{ std::roundf(p.x + g.offset.x), p.y + g.offset.y };
    Game::renderer()->rgba_sprite_renderer().DrawSprite(g.sprite, draw_loc);
    p.x += g.advance;
  }
}

void SpriteFont::DrawTextWithContrast(const std::string& text, const Float2& location, const Color& fg, const Color& bg, int32_t offset) {
  if (offset > 0) {
    DrawText(text, location + Float2(static_cast<float>(-offset), 0.0f), bg);
    DrawText(text, location + Float2(static_cast<float>(offset), 0.0f), bg);
    DrawText(text, location + Float2(0.0f, static_cast<float>(-offset)), bg);
    DrawText(text, location + Float2(0.0f, static_cast<float>(offset)), bg);
  }
  DrawText(text, location, fg);
}

Size SpriteFont::Measure(const std::string& text) {
  if (text.empty()) {
    return{ 0, size_ };
  }

  auto lines = StringUtils::Split(text, '\n');
  std::vector<float> widths;
  std::transform(lines.begin(), lines.end(), std::back_inserter(widths), [this](const auto& s) { return LineWidth(s); });
  auto max_width = static_cast<int32_t>(*std::max_element(widths.begin(), widths.end()));
  return{ max_width, static_cast<int32_t>(lines.size() * size_) };
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
      glyphs_[std::make_pair(c, color)];
    }
  });
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
    { min_x, -max_y },
    builder_.Allocate({ max_x - min_x, max_y - min_y })
  };

  SDL_LockSurface(surface.get());

  auto& s = g.sprite;
  int32_t* p = static_cast<int32_t*>(surface->pixels);
  char* dest = &s.sheet->GetData()[0];
  auto dest_stride = s.sheet->size().width * 4;
  for (auto j = 0; j < s.size.y; ++j) {
    for (auto i = 0; i < s.size.x; ++i) {
      auto x = i + min_x;
      auto y = j + TTF_FontAscent(ttf_font_.get()) - max_y;
      Color cc(*(p + (y * surface->pitch >> 2) + x));
      if (cc.a != 0) {
        auto q = dest_stride * (j + s.bounds.Top()) + 4 * (i + s.bounds.Left());
        auto pmc = GraphicsUtil::PremultiplyAlpha(Color(cc.a, color));

        dest[q] = pmc.b;
        dest[q + 1] = pmc.g;
        dest[q + 2] = pmc.r;
        dest[q + 3] = pmc.a;
      }
    }
  }

  SDL_UnlockSurface(surface.get());
  surface = nullptr;

  s.sheet->CommitBufferedData();

  return g;
}

float SpriteFont::LineWidth(const std::string& line) {
  auto sum = [this](float sum, char c) { return sum + glyphs_[std::make_pair(c, Color::White)].advance; };
  return std::accumulate(line.begin(), line.end(), 0.0f, sum);
}

}