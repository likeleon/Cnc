#pragma once

#include "cnc/widget.h"
#include "cnc/color.h"

namespace cnc {

struct MouseInput;

namespace mods {
namespace common {

class ButtonWidget : public Widget {
public:
  ButtonWidget();

  WidgetPtr Clone() const override;

  static void DrawBackground(const std::string& base_name,
                             const Rectangle& rect,
                             bool disabled, bool pressed, bool hover, bool highlighted);

  bool YieldMouseFocus(const MouseInput& mi);
  bool HandleMouseInput(const MouseInput& mi);

  Point ChildOrigin() const override;

  void Draw() override;
  virtual int UsableWidth() const;

  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string tooltip_container_;
  std::string key_;
  std::string text_;
  std::string background_ = "button";
  bool depressed_ = false;
  int32_t visual_height_;
  int32_t base_line_;
  std::string font_;
  Color text_color_;
  Color text_color_disabled_;
  bool disabled_ = false;
  bool highlighted_ = false;
  Func<std::string> get_text_;
  Func<Color> get_color_;
  Func<Color> get_color_disabled_;
  Func<bool> is_disabled_;
  Func<bool> is_highlighted_;
  Action<const MouseInput&> on_mouse_down_ = [](const auto&) {};
  Action<const MouseInput&> on_mouse_up_ = [](const auto&) {};
  Action<> on_click_ = [] {};
  Action<> on_double_click_ = [] {};

protected:
  ButtonWidget(const ButtonWidget& other);

private:
  void DrawBackground(const Rectangle& rect, bool disabled, bool pressed, bool hover, bool highlighted);
};

using ButtonWidgetPtr = std::shared_ptr<ButtonWidget>;

}
}
}
