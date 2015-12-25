#pragma once

#include "cnc/size.h"
#include "cnc/itexture.h"

namespace cnc {

enum class SheetType {
  Indexed = 1,
  DualIndexed = 2,
  BGRA = 4
};

class CNC_API Sheet {
public:
  Sheet(SheetType type, ITexturePtr texture);
  Sheet(SheetType type, const std::string& stream);

  ITexturePtr GetTexture();
  void CreateBuffer();
  void ReleaseBuffer();
  bool Buffered() const;
  std::string& GetData();

  SheetType type() const;
  const Size& size() const;

private:
  SheetType type_;
  Size size_;
  bool dirty_ = false;
  bool release_buffer_on_commit_ = false;
  ITexturePtr texture_;
  std::string data_;
};

}
