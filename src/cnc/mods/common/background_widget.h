#pragma once

#include "cnc/widget.h"

namespace cnc {
namespace mods {
namespace common {

class BackgroundWidget : public Widget {
public:
  BackgroundWidget(const WidgetArgs& args);
  
  const FieldInfo* GetFieldInfo(const std::string& name) const override;

  std::string background;
};

}
}
}