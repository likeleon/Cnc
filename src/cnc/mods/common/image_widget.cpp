#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/image_widget.h"
#include "cnc/chrome_provider.h"
#include "cnc/widget_utils.h"
#include "cnc/iinput_handler.h"
#include "cnc/float2.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> ImageWidget::GetFieldInfoMap() const {
  return{
    { "ImageCollection", TypeFieldInfo(&ImageWidget::image_collection_) },
    { "ImageName", TypeFieldInfo(&ImageWidget::image_name_) }
  };
}

ImageWidget::ImageWidget()
  : get_image_name_([this]() { return image_name_; }),
  get_image_collection_([this]() { return image_collection_; }) {
}

ImageWidget::ImageWidget(const ImageWidget& /*other*/) = default;

WidgetPtr ImageWidget::Clone() const {
  return WidgetPtr(new ImageWidget(*this));
}

void ImageWidget::Draw() {
  auto name = get_image_name_();
  auto collection = get_image_collection_();

  auto sprite = ChromeProvider::GetImage(collection, name);
  if (sprite == nullptr) {
    std::ostringstream oss;
    oss << "Sprite " << collection << "/" << name << " was not found.";
    throw Error(MSG(oss.str()));
  }

  WidgetUtils::DrawRGBA(*sprite, RenderOrigin());
}

bool ImageWidget::HandleMouseInput(const MouseInput& mi) {
  return !click_through_ && RenderBounds().Contains(mi.location);
}

}
}
}
