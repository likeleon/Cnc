#include "cnc/mods/common/stdafx.h"
#include "cnc/field_loader.h"
#include "cnc/mods/common/label_widget.h"

namespace cnc {
namespace mods {
namespace common {

const std::string TextAlignTraits::pretty_name = "TextAlign";

const EnumNamesType<TextAlignTraits> TextAlignTraits::names = {
  { TextAlign::Left, "Left" },
  { TextAlign::Center, "Center" },
  { TextAlign::Right, "Right" }
};

std::map<std::string, FieldInfo> LabelWidget::GetFieldInfoMap() const {
  return {
    { "Align", EnumFieldInfo<TextAlignTraits>(&LabelWidget::align_) },
  };
}

}
}
}
