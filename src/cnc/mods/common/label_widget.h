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

struct TextAlignTraits {
  using E = TextAlign;
  static const std::string pretty_name;
  static const EnumNamesType<TextAlignTraits> names;
};

class LabelWidget : public Widget {
public:
  LabelWidget();

  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string text_;
  TextAlign align_ = TextAlign::Left;
  std::string font_;
};

}
}
}
