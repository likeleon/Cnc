#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/progress_bar_widget.h"
#include "cnc/widget_utils.h"
#include "cnc/chrome_provider.h"
#include "cnc/sprite.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> ProgressBarWidget::GetFieldInfoMap() const {
  return{
    { "Background", TypeFieldInfo(&ProgressBarWidget::background_) },
    { "Bar", TypeFieldInfo(&ProgressBarWidget::bar_) },
    { "BarMargin", TypeFieldInfo(&ProgressBarWidget::bar_margin_) }
  };
}

ProgressBarWidget::ProgressBarWidget() {
  get_percentage_ = [this]() { return percentage_; };
  is_indeterminate_ = [this]() { return indeterminate_; };
}

ProgressBarWidget::ProgressBarWidget(const ProgressBarWidget& other)
 : Widget(other) {
  percentage_ = other.percentage_;
  get_percentage_ = other.get_percentage_;
  is_indeterminate_ = other.is_indeterminate_;
}

WidgetPtr ProgressBarWidget::Clone() const {
  return std::shared_ptr<ProgressBarWidget>(new ProgressBarWidget(*this));
}

void ProgressBarWidget::Draw() {
  auto rb = RenderBounds();
  auto percentage = get_percentage_();
  WidgetUtils::DrawPanel(background_, rb);

  auto min_bar_width = static_cast<int32_t>(ChromeProvider::GetImage(bar_, "border-l")->size.x + ChromeProvider::GetImage(bar_, "border-r")->size.x);
  auto max_bar_width = rb.width - bar_margin_.width * 2;
  auto bar_width = was_indeterminate_ ? max_bar_width / 4 : percentage * max_bar_width / 100;
  bar_width = std::max<int32_t>(bar_width, min_bar_width);

  auto bar_offset = was_indeterminate_ ? static_cast<int32_t>(0.75 * offset_ * max_bar_width) : 0;
  Rectangle bar_rect(rb.x + bar_margin_.width + bar_offset, rb.y + bar_margin_.height, bar_width, rb.height - 2 * bar_margin_.height);
  WidgetUtils::DrawPanel(bar_, bar_rect);
}

}
}
}
