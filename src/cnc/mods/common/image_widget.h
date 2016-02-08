#pragma once

#include "cnc/widget.h"

namespace cnc {
namespace mods {
namespace common {

class ImageWidget : public Widget {
public:
  ImageWidget();
  ImageWidget(const ImageWidget& other);

  WidgetPtr Clone() const override;
  void Draw() override;
  bool HandleMouseInput(const MouseInput& mi) override;

  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string image_collection_;
  std::string image_name_;
  bool click_through_ = true;
  Func<std::string> get_image_name_;
  Func<std::string> get_image_collection_;
};

}
}
}
