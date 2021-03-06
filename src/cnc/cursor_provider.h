#pragma once

#include "cnc/cursor_sequence.h"

namespace cnc {

class ModData;

class CNC_API CursorProvider {
public:
  CursorProvider(ModData& mod_data);

  CursorProvider(const CursorProvider&) = delete;
  CursorProvider& operator=(const CursorProvider&) = delete;

  static bool CursorViewportZoomed();

  bool HasCursorSequence(const std::string& cursor) const;
  const CursorSequence& GetCursorSequence(const std::string& cursor) const;
  const std::map<std::string, CursorSequence>& cursors() const;
  const std::map<std::string, ImmutablePalettePtr>& palettes() const;

private:
  std::map<std::string, CursorSequence> cursors_;
  std::map<std::string, ImmutablePalettePtr> palettes_;
  ModData& mod_data_;
};

}
