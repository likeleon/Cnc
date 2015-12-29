#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/background_widget.h"

namespace cnc {
namespace mods {
namespace common {

BackgroundWidget::BackgroundWidget(const WidgetArgs& /*args*/) {
}

const FieldInfo* BackgroundWidget::OnGetFieldInfo(const std::string& name) const {
  static const std::map<std::string, FieldInfo> BackgroundWidgetFieldInfo = {
    { "Background", StringFieldInfo(&BackgroundWidget::background) },
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