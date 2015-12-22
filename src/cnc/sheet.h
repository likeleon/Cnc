#pragma once

#include "cnc/size.h"

namespace cnc {

enum class SheetType {
  Indexed = 1,
  DualIndexed = 2,
  BGRA = 4
};

class CNC_API Sheet {
public:
  Sheet(SheetType type, const std::string& stream);

  void CreateBuffer();
  void ReleaseBuffer();
  bool Buffered() const;
  std::string& GetData();

  const Size& size() const;

private:
  SheetType type_;
  Size size_;
  std::string data_;
  bool dirty_ = false;
  bool release_buffer_on_commit_ = false;
};

}
