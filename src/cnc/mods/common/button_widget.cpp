#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/button_widget.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/chrome_metrics.h"
#include "cnc/widget_utils.h"
#include "cnc/point.h"
#include "cnc/sprite_font.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> ButtonWidget::GetFieldInfoMap() const {
  return{
    { "TooltipContainer", TypeFieldInfo(&ButtonWidget::tooltip_container_) },
    { "Key", TypeFieldInfo(&ButtonWidget::key_) },
    { "Text", TypeFieldInfo(&ButtonWidget::text_) },
    { "Background", TypeFieldInfo(&ButtonWidget::background_) },
    { "Font", TypeFieldInfo(&ButtonWidget::font_) }
  };
}

ButtonWidget::ButtonWidget()
  : visual_height_(ChromeMetrics::Get<int32_t>("ButtonDepth")),
  base_line_(ChromeMetrics::Get<int32_t>("ButtonBaseLine")),
  font_(ChromeMetrics::Get<std::string>("ButtonFont")),
  text_color_(ChromeMetrics::Get<Color>("ButtonTextColor")),
  text_color_disabled_(ChromeMetrics::Get<Color>("ButtonTextColorDisabled")),
  get_text_([this] { return text_; }),
  get_color_([this] { return text_color_; }),
  get_color_disabled_([this] { return text_color_disabled_; }),
  on_mouse_up_([this](const auto&) { on_click_(); }),
  is_disabled_([this] { return disabled_; }),
  is_highlighted_([this] { return highlighted_; }) {
}

ButtonWidget::ButtonWidget(const ButtonWidget& other)
  : Widget(other),
  tooltip_container_(other.tooltip_container_),
  text_(other.text_),
  background_(other.background_),
  depressed_(other.depressed_),
  visual_height_(other.visual_height_),
  base_line_(other.base_line_),
  font_(other.font_),
  text_color_(other.text_color_),
  text_color_disabled_(other.text_color_disabled_),
  disabled_(other.disabled_),
  highlighted_(other.highlighted_),
  get_text_(other.get_text_),
  get_color_(other.get_color_),
  get_color_disabled_(other.get_color_disabled_),
  is_disabled_(other.is_disabled_),
  is_highlighted_(other.is_highlighted_),
  on_mouse_down_(other.on_mouse_down_),
  on_mouse_up_([this](const auto&) { on_click_(); }) {
}

WidgetPtr ButtonWidget::Clone() const {
  return ButtonWidgetPtr(new ButtonWidget(*this));
}

bool ButtonWidget::YieldMouseFocus(const MouseInput& mi) {
  depressed_ = false;
  return Widget::YieldMouseFocus(mi);
}

bool ButtonWidget::HandleMouseInput(const MouseInput& mi) {
  if (mi.button != MouseButton::Left) {
    return false;
  }

  if (mi.event == MouseInputEvent::Down && !TakeMouseFocus(mi)) {
    return false;
  }

  auto disabled = is_disabled_();
  if (HasMouseFocus() && mi.event == MouseInputEvent::Up && mi.multi_tap_count == 2) {
    if (!disabled) {
      on_double_click_();
      return YieldMouseFocus(mi);
    }
  } else if (HasMouseFocus() && mi.event == MouseInputEvent::Up) {
    if (depressed_ && !disabled) {
      on_mouse_up_(mi);
    }

    return YieldMouseFocus(mi);
  }

  if (mi.event == MouseInputEvent::Down) {
    if (!disabled) {
      on_mouse_down_(mi);
      depressed_ = true;
    } else {
      YieldMouseFocus(mi);
    }
  } else if (mi.event == MouseInputEvent::Move && HasMouseFocus()) {
    depressed_ = RenderBounds().Contains(mi.location);
  }

  return depressed_;
}

Point ButtonWidget::ChildOrigin() const {
  return RenderOrigin() +
    (depressed_ ? Point(visual_height_, visual_height_) : Point::Zero);
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
