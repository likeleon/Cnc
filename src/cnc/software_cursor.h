#pragma once

#include "cnc/icursor.h"
#include "cnc/sheet_builder.h"
#include "cnc/sprite.h"
#include "cnc/hardware_palette.h"

namespace cnc {

class CursorProvider;

class SoftwareCursor : public ICursor {
public:
  SoftwareCursor(CursorProvider& cursor_provider);

  void Render(Renderer& renderer) override;
  void SetCursor(const std::string& cursor) override;
  void Tick() override;

private:
  HardwarePalette palette_;
  CursorProvider& cursor_provider_;
  std::string cursor_name_;
  float cursor_frame_ = 0.0f;
  SheetBuilder sheet_builder_;
  std::map<std::string, std::vector<Sprite>> sprites_;
};

}