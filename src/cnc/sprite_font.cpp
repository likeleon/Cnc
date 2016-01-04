#include "cnc/stdafx.h"
#include "cnc/sprite_font.h"
#include "cnc/sheet_builder.h"
#include "cnc/error.h"
#include "cnc/perf_timer.h"

namespace cnc {

SpriteFont::SpriteFont(const std::string& name, int32_t size, SheetBuilder& builder)
  : size_(size), builder_(builder) {
  if (builder_.type() != SheetType::BGRA) {
    throw Error(MSG("The sheet builder must create BGRA sheets"));
  }

  // TODO: TTF

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

SpriteFont::GlyphInfo SpriteFont::CreateGlyph(const std::pair<char, Color>& /*c*/) {
  return{ 0.0f, Point::Zero };
}

float SpriteFont::LineWidth(const std::string& line) {
  return std::accumulate(line.begin(), line.end(),
                         0.0f,
                         [this](float sum, char c) { return sum + Glyph(c, Color::White).advance; });
}

}