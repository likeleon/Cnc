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
  text_color_(ChromeMetrics::Get<Color>("TextColor")) {
}

void LabelWidget::Draw() {
  auto iter = Game::renderer()->fonts().find(font_);
  if (iter == Game::renderer()->fonts().end()) {
    throw Error(MSG("Requested font '" + font_ + "' was not found."));
  }

  auto font = iter->second.get();
  if (text_.empty()) {
    return;
  }

  auto text_size = font->Measure(text_);
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
  
  font->DrawText(text_, position, text_color_);
}

std::map<std::string, FieldInfo> LabelWidget::GetFieldInfoMap() const {
  return{
    { "Text", StringFieldInfo(&LabelWidget::text_) },
    { "Align", EnumFieldInfo<TextAlignTraits>(&LabelWidget::align_) },
    { "VAlign", EnumFieldInfo<TextVAlignTraits>(&LabelWidget::valign_) },
    { "Font", StringFieldInfo(&LabelWidget::font_) }
  };
}

}
}
}
