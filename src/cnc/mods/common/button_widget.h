#pragma once

#include "cnc/widget.h"
#include "cnc/color.h"

namespace cnc {

struct MouseInput;

namespace mods {
namespace common {

template <typename... T>
using Action = std::function<void(T...)>;

template <typename TResult, typename... T>
using Func = std::function<TResult(T...)>;

class ButtonWidget : public Widget {
public:
  ButtonWidget();

  static void DrawBackground(const std::string& base_name,
                             const Rectangle& rect,
                             bool disabled, bool pressed, bool hover, bool highlighted);

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
  Action<const MouseInput&> on_mouse_down = [](const auto&) {};
  Action<const MouseInput&> on_mouse_up = [](const auto&) {};
  Action<> on_click_ = [] {};

private:
  void DrawBackground(const Rectangle& rect, bool disabled, bool pressed, bool hover, bool highlighted);
};

using ButtonWidgetPtr = std::shared_ptr<ButtonWidget>;

}
}
}
