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


class LabelWidget : public Widget {
public:
  const FieldInfo* OnGetFieldInfo(const std::string& name) const override;

  TextAlign align_ = TextAlign::Left;
};

}
}
}

namespace cnc {

template <>
struct EnumInfoTraits<mods::common::TextAlign> {
  static const std::string pretty_name;
  static const EnumInfo<mods::common::TextAlign>::NamesType names;
};

}