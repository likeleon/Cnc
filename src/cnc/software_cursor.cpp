#include "cnc/stdafx.h"
#include "cnc/software_cursor.h"
#include "cnc/cursor_provider.h"
#include "cnc/cursor_sequence.h"
#include "cnc/sheet.h"
#include "cnc/game.h"
#include "cnc/renderer.h"

namespace cnc {

SoftwareCursor::SoftwareCursor(CursorProvider& cursor_provider)
  : cursor_provider_(cursor_provider), sheet_builder_(SheetType::Indexed) {
  for (const auto& kv : cursor_provider_.cursors()) {
    std::vector<Sprite> s;
    for (const auto& f : kv.second.frames()) {
      s.emplace_back(sheet_builder_.Add(*f));
    }
    sprites_.emplace(kv.first, std::move(s));
  }
  sheet_builder_.current()->ReleaseBuffer();
}

void SoftwareCursor::Render(Renderer& /*renderer*/) {
  if (cursor_name_.empty()) {
    return;
  }

  auto& cursor_sequence = cursor_provider_.GetCursorSequence(cursor_name_);
  auto& cursor_sprite = sprites_[cursor_name_][static_cast<int32_t>(cursor_frame_) % cursor_sequence.length()];
  auto cursor_size = CursorProvider::CursorViewportZoomed() ? 2.0f * cursor_sprite.size : cursor_sprite.size;

  auto cursor_offset = CursorProvider::CursorViewportZoomed()
    ? (2 * cursor_sequence.hotspot()) + cursor_sprite.size.ToPoint()
    : cursor_sequence.hotspot() + (0.5f * cursor_sprite.size).ToPoint();

  // TODO
}

void SoftwareCursor::SetCursor(const std::string& cursor) {
  cursor_name_ = cursor;
}

void SoftwareCursor::Tick() {
  cursor_frame_ += 0.5f;
}

}
