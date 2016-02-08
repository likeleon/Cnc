#include "cnc/mods/common/stdafx.h"
#include "cnc/field_loader.h"
#include "cnc/chrome_metrics.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/sprite_font.h"
#include "cnc/mods/common/label_widget.h"

namespace cnc {
namespace mods {
namespace common {

const std::string TextAlignTraits::pretty_name = "TextAlign";

const EnumNamesType<TextAlignTraits> TextAlignTraits::names = {
  { TextAlign::Left, "Left" },
  { TextAlign::Center, "Center" },
  { TextAlign::Right, "Right" }
};

const std::string TextVAlignTraits::pretty_name = "TextVAlign";

const EnumNamesType<TextVAlignTraits> TextVAlignTraits::names = {
  { TextVAlign::Top, "Top" },
  { TextVAlign::Middle, "Middle" },
  { TextVAlign::Bottom, "Bottom" }
};

LabelWidget::LabelWidget()
  : font_(ChromeMetrics::Get<std::string>("TextFont")),
  text_color_(ChromeMetrics::Get<Color>("TextColor")),
  contrast_(ChromeMetrics::Get<bool>("TextContrast")),
  contrast_color_(ChromeMetrics::Get<Color>("TextContrastColor")),
  get_text_([this]() { return text_; }),
  get_color_([this]() { return text_color_; }),
  get_contrast_color_([this]() { return contrast_color_; }) {
}

LabelWidget::LabelWidget(const LabelWidget& /*other*/) = default;

WidgetPtr LabelWidget::Clone() const {
  return WidgetPtr(new LabelWidget(*this));
}

void LabelWidget::Draw() {
  auto iter = Game::renderer()->fonts().find(font_);
  if (iter == Game::renderer()->fonts().end()) {
    throw Error(MSG("Requested font '" + font_ + "' was not found."));
  }
  auto font = iter->second.get();

  auto text = get_text_();
  if (text.empty()) {
    return;
  }

  auto text_size = font->Measure(text);
  auto position = RenderOrigin();

  if (valign_ == TextVAlign::Middle) {
    position += { 0, (bounds().height - text_size.height) / 2 };
  } else if (valign_ == TextVAlign::Bottom) {
    position += { 0, (bounds().height - text_size.height) };
  }

  if (align_ == TextAlign::Center) {
    position += { (bounds().width - text_size.width) / 2, 0 };
  } else if (align_ == TextAlign::Right) {
    position += { (bounds().width - text_size.width), 0 };
  }

  if (word_wrap_) {
    throw Error(MSG("TODO: LabelWidget::word_wrap"));
  }

  auto color = get_color_();
  auto contrast = get_contrast_color_();
  if (contrast_) {
    font->DrawTextWithContrast(text, position, color, contrast, 2);
  } else {
    font->DrawText(text, position, color);
  }
}

std::map<std::string, FieldInfo> LabelWidget::GetFieldInfoMap() const {
  return{
    { "Text", TypeFieldInfo(&LabelWidget::text_) },
    { "Align", EnumFieldInfo<TextAlignTraits>(&LabelWidget::align_) },
    { "VAlign", EnumFieldInfo<TextVAlignTraits>(&LabelWidget::valign_) },
    { "Font", TypeFieldInfo(&LabelWidget::font_) }
  };
}

}
}
}
