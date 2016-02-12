#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/background_widget.h"
#include "cnc/widget_utils.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> BackgroundWidget::GetFieldInfoMap() const {
  return {
    { "Background", TypeFieldInfo(&BackgroundWidget::background_) },
  };
}

void BackgroundWidget::Draw() {
  WidgetUtils::DrawPanel(background_, RenderBounds());
}

}
}
}
