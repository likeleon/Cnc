#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/button_widget.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/chrome_metrics.h"
#include "cnc/widget_utils.h"
#include "cnc/point.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> ButtonWidget::GetFieldInfoMap() const {
  return{
    { "TooltipContainer", StringFieldInfo(&ButtonWidget::tooltip_container_) },
    { "Key", StringFieldInfo(&ButtonWidget::key_) },
    { "Text", StringFieldInfo(&ButtonWidget::text_) },
    { "Background", StringFieldInfo(&ButtonWidget::background_) }
  };
}

ButtonWidget::ButtonWidget()
  : visual_height_(ChromeMetrics::Get<int32_t>("ButtonDepth")),
    base_line_(ChromeMetrics::Get<int32_t>("ButtonBaseLine")),
    font_(ChromeMetrics::Get<std::string>("ButtonFont")),
    text_color_(ChromeMetrics::Get<Color>("ButtonTextColor")),
    text_color_disabled_(ChromeMetrics::Get<Color>("ButtonTextColorDisabled")) {
  get_text_ = [this] { return text_; };
  get_color_ = [this] { return text_color_; };
  get_color_disabled_ = [this] { return text_color_disabled_; };
  on_mouse_up = [this](const auto&) { on_click_(); };
  is_disabled_ = [this] { return disabled_; };
  is_highlighted_ = [this] { return highlighted_; };
}

void ButtonWidget::Draw() {
  auto rb = RenderBounds();
  auto disabled = is_disabled_();
  auto highlighted = is_highlighted_();

  auto font = Game::renderer()->fonts().at(font_).get();
  auto text = get_text_();
  auto color = get_color_();
  auto color_disabled = get_color_disabled_();
  auto s = font->Measure(text);
  Point state_offset = depressed_ ? Point(visual_height_, visual_height_) : Point::Zero;
  Point position = { 
    rb.x + (UsableWidth() - s.width) / 2, 
    rb.y - base_line_ + (bounds().height - s.height) / 2 
  };

  DrawBackground(rb, disabled, depressed_, Ui::mouse_over_widget().get() == this, highlighted);
  font->DrawText(text, position + state_offset, disabled ? color_disabled : color);
}

int ButtonWidget::UsableWidth() const {
  return bounds().width;
}

void ButtonWidget::DrawBackground(const Rectangle& rect, bool disabled, bool pressed, bool hover, bool highlighted) {
  DrawBackground(background_, rect, disabled, pressed, hover, highlighted);
}

void ButtonWidget::DrawBackground(const std::string& base_name,
                                  const Rectangle& rect,
                                  bool disabled, bool pressed, bool hover, bool highlighted) {
  auto variant = highlighted ? "-highlighted" : "";
  auto state = disabled ? "-disabled" :
               pressed ? "-pressed" :
               hover ? "-hover" :
               "";
  
  WidgetUtils::DrawPanel(base_name + variant + state, rect);
}

}
}
}
