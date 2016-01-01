#include "cnc/mods/common/stdafx.h"
#include "cnc/field_loader.h"
#include "cnc/mods/common/label_widget.h"

namespace cnc {
namespace mods {
namespace common {

const FieldInfo* LabelWidget::OnGetFieldInfo(const std::string& name) const {

  static const std::map<std::string, FieldInfo> BackgroundWidgetFieldInfo = {
    { "Align", EnumFieldInfo(&LabelWidget::align_) },
  };
  auto kvp = BackgroundWidgetFieldInfo.find(name);
  if (kvp == BackgroundWidgetFieldInfo.end()) {
    return nullptr;
  }
  return &kvp->second;
}

}
}
}

namespace cnc {

using TextAlign = mods::common::TextAlign;

const std::string EnumInfoTraits<TextAlign>::pretty_name = "TextAlign";

const EnumInfo<mods::common::TextAlign>::NamesType EnumInfoTraits<TextAlign>::names = {
  { TextAlign::Left, "Left" },
  { TextAlign::Center, "Center" },
  { TextAlign::Right, "Right" }
};

}