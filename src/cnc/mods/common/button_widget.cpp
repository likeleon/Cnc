#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/button_widget.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> ButtonWidget::GetFieldInfoMap() const {
  return{
    { "TooltipContainer", StringFieldInfo(&ButtonWidget::tooltip_container_) },
    { "Key", StringFieldInfo(&ButtonWidget::key_) },
    { "Text", StringFieldInfo(&ButtonWidget::text_) },
    { "Background", StringFieldInfo(&ButtonWidget::background_) }
  };
}

}
}
}
