#pragma once

#include "cnc/widget.h"
#include "cnc/enum_info.h"
#include "cnc/color.h"

namespace cnc {
namespace mods {
namespace common {

enum class TextAlign {
  Left,
  Center,
  Right
};

enum class TextVAlign {
  Top,
  Middle,
  Bottom
};

struct TextAlignTraits {
  using E = TextAlign;
  static const std::string pretty_name;
  static const EnumNamesType<TextAlignTraits> names;
};

struct TextVAlignTraits {
  using E = TextVAlign;
  static const std::string pretty_name;
  static const EnumNamesType<TextVAlignTraits> names;
};

class LabelWidget : public Widget {
public:
  LabelWidget();
  LabelWidget(const LabelWidget& other);

  WidgetPtr Clone() const override;
  void Draw() override;

  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string text_;
  TextAlign align_ = TextAlign::Left;
  TextVAlign valign_ = TextVAlign::Middle;
  std::string font_;
  Color text_color_;
  bool contrast_;
  Color contrast_color_;
  bool word_wrap_ = false;
  Func<std::string> get_text_;
  Func<Color> get_color_;
  Func<Color> get_contrast_color_;
};

}
}
}
