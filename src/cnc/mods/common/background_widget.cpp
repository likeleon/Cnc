#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/background_widget.h"

namespace cnc {
namespace mods {
namespace common {

BackgroundWidget::BackgroundWidget(const WidgetArgs& /*args*/) {
}

const FieldInfo* BackgroundWidget::GetFieldInfo(const std::string& name) const {
  static const std::map<std::string, FieldInfo> BackgroundWidgetFieldInfo = {
    { "Background", StringFieldInfo(&BackgroundWidget::background) },
  };
  auto kvp = BackgroundWidgetFieldInfo.find(name);
  if (kvp != BackgroundWidgetFieldInfo.end()) {
    return &kvp->second;
  }
  return Widget::GetFieldInfo(name);
}

}
}
}