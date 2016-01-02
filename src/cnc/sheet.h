#pragma once

#include "cnc/size.h"
#include "cnc/itexture.h"
#include "cnc/sheet_builder.h"

namespace cnc {

class CNC_API Sheet {
public:
  Sheet(SheetType type, ITexturePtr texture);
  Sheet(SheetType type, const std::vector<char>& stream);

  ITexturePtr GetTexture();
  void CreateBuffer();
  void ReleaseBuffer();
  bool Buffered() const;
  std::vector<char>& GetData();

  SheetType type() const;
  const Size& size() const;

private:
  SheetType type_;
  Size size_;
  bool dirty_ = false;
  bool release_buffer_on_commit_ = false;
  ITexturePtr texture_;
  std::vector<char> data_;
};

}
