#pragma once

namespace cnc {

class SheetBuilder;

class SpriteFont {
public:
  SpriteFont(const std::string& name, int32_t size, SheetBuilder& builder);

private:
  int32_t size_;
  SheetBuilder& builder_;
};

using SpriteFontPtr = std::unique_ptr<SpriteFont>;

}
