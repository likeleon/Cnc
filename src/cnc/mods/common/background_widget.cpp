#include "cnc/mods/common/stdafx.h"
#include "cnc/widget_utils.h"
#include "cnc/mods/common/background_widget.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> BackgroundWidget::GetFieldInfoMap() const {
  return {
    { "Background", StringFieldInfo(&BackgroundWidget::background_) },
  };
}

void BackgroundWidget::Draw() {
  WidgetUtils::DrawPanel(background_, RenderBounds());
}

}
}
}
