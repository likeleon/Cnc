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
  const FieldInfo* OnGetFieldInfo(const std::string& name) const override;

  TextAlign align_ = TextAlign::Left;
};

}
}
}
