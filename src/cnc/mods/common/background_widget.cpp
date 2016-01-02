#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/background_widget.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> BackgroundWidget::GetFieldInfoMap() const {
  return {
    { "Background", StringFieldInfo(&BackgroundWidget::background_) },
  };
}

}
}
}
