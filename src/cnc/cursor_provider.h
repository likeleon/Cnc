#pragma once

#include "cnc/cursor_sequence.h"

namespace cnc {

class ModData;

class CNC_API CursorProvider {
public:
  CursorProvider(ModData& mod_data);

  CursorProvider(const CursorProvider&) = delete;
  CursorProvider& operator=(const CursorProvider&) = delete;

  bool HasCursorSequence(const std::string& cursor) const;
  const CursorSequence& GetCursorSequence(const std::string& cursor) const;
  const std::map<std::string, CursorSequence>& cursors() const;

private:
  std::map<std::string, CursorSequence> cursors_;
  ModData& mod_data_;
};

}
