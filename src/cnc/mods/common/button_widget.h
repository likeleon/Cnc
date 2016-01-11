#pragma once

#include "cnc/widget.h"

namespace cnc {
namespace mods {
namespace common {

class ButtonWidget : public Widget {
public:
  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string tooltip_container_;
  std::string key_;
  std::string text_;
  std::string background_;
};

using ButtonWidgetPtr = std::shared_ptr<ButtonWidget>;

}
}
}
