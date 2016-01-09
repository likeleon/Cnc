#pragma once

#include "cnc/widget.h"

namespace cnc {
namespace mods {
namespace common {

class BackgroundWidget : public Widget {
public:
  void Draw() override;

  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string background_ = "dialog";
};

}
}
}
