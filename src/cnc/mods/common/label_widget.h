#pragma once

#include "cnc/widget.h"
#include "cnc/enum_info.h"

namespace cnc {
namespace mods {
namespace common {

enum class TextAlign {
  Left,
  Center,
  Right
};

enum class TextVAlign {
  Top,
  Middle,
  Bottom
};

struct TextAlignTraits {
  using E = TextAlign;
  static const std::string pretty_name;
  static const EnumNamesType<TextAlignTraits> names;
};

struct TextVAlignTraits {
  using E = TextVAlign;
  static const std::string pretty_name;
  static const EnumNamesType<TextVAlignTraits> names;
};

class LabelWidget : public Widget {
public:
  LabelWidget();

  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string text_;
  TextAlign align_ = TextAlign::Left;
  TextVAlign valign_ = TextVAlign::Middle;
  std::string font_;
};

}
}
}
