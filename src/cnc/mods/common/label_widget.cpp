#include "cnc/mods/common/stdafx.h"
#include "cnc/field_loader.h"
#include "cnc/chrome_metrics.h"
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

LabelWidget::LabelWidget()
: font_(ChromeMetrics::Get<std::string>("TextFont")) {
}

std::map<std::string, FieldInfo> LabelWidget::GetFieldInfoMap() const {
  return {
    { "Text", StringFieldInfo(&LabelWidget::text_) },
    { "Align", EnumFieldInfo<TextAlignTraits>(&LabelWidget::align_) },
    { "Font", StringFieldInfo(&LabelWidget::font_) }
  };
}

}
}
}
