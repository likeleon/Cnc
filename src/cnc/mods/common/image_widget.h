#pragma once

#include "cnc/widget.h"

namespace cnc {
namespace mods {
namespace common {

class ImageWidget : public Widget {
public:
  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string image_collection_;
  std::string image_name_;
};

}
}
}
