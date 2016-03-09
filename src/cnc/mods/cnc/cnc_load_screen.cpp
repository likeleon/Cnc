#include "cnc/mods/cnc/stdafx.h"
#include "cnc/mods/cnc/cnc_load_screen.h"
#include "cnc/game.h"
#include "cnc/renderer.h"
#include "cnc/file.h"
#include "cnc/platform.h"
#include "cnc/sheet.h"
#include "cnc/manifest.h"
#include "cnc/mod_data.h"
#include "cnc/widget_utils.h"
#include "cnc/sprite_font.h"
#include "cnc/sprite.h"
#include "cnc/color.h"

namespace cnc {
namespace mods {
namespace cnc {

CncLoadScreen::CncLoadScreen() = default;
CncLoadScreen::~CncLoadScreen() = default;

void CncLoadScreen::Init(const Manifest& m, const std::map<std::string, std::string>& info) {
  load_info_ = info;

  r_ = Game::renderer();
  if (r_ == nullptr) {
    return;
  }

  auto stream = File::OpenRead(Platform::ResolvePath(load_info_["Image"]));
  sheet_ = std::make_shared<Sheet>(SheetType::BGRA, stream);

  auto res = r_->Resolution();
  border_top_ = std::make_unique<Sprite>(sheet_, Rectangle(161, 128, 62, 33), TextureChannel::Alpha);
  border_bottom_ = std::make_unique<Sprite>(sheet_, Rectangle(161, 223, 62, 33), TextureChannel::Alpha);
  border_left_ = std::make_unique<Sprite>(sheet_, Rectangle(128, 161, 33, 62), TextureChannel::Alpha);
  border_right_ = std::make_unique<Sprite>(sheet_, Rectangle(223, 161, 33, 62), TextureChannel::Alpha);
  corner_top_left_ = std::make_unique<Sprite>(sheet_, Rectangle(128, 128, 33, 33), TextureChannel::Alpha);
  corner_top_right_ = std::make_unique<Sprite>(sheet_, Rectangle(223, 128, 33, 33), TextureChannel::Alpha);
  corner_bottom_left_ = std::make_unique<Sprite>(sheet_, Rectangle(128, 223, 33, 33), TextureChannel::Alpha);
  corner_bottom_right_ = std::make_unique<Sprite>(sheet_, Rectangle(223, 223, 33, 33), TextureChannel::Alpha);

  nod_logo_ = std::make_unique<Sprite>(sheet_, Rectangle(0, 256, 256, 256), TextureChannel::Alpha);
  gdi_logo_ = std::make_unique<Sprite>(sheet_, Rectangle(256, 256, 256, 256), TextureChannel::Alpha);
  eva_logo_ = std::make_unique<Sprite>(sheet_, Rectangle(256, 64, 128, 64), TextureChannel::Alpha);
  nod_pos_ = { bounds_.width / 2 - 384, bounds_.height / 2 - 128 };
  gdi_pos_ = { bounds_.width / 2 + 128, bounds_.height / 2 - 128 };
  eva_pos_ = { bounds_.width / 2 - 43 - 128, 43 };

  bright_block_ = std::make_unique<Sprite>(sheet_, Rectangle(320, 0, 16, 35), TextureChannel::Alpha);
  dim_block_ = std::make_unique<Sprite>(sheet_, Rectangle(336, 0, 16, 35), TextureChannel::Alpha);

  version_text_ = m.mod().version;
}

void CncLoadScreen::Display() {
  if (r_ == nullptr || load_timer_.ElapsedMilliseconds() < 250) {
    return;
  }

  load_timer_.Restart();

  load_tick_ = ++load_tick_ % 8;
  r_->BeginFrame(Point::Zero, 1.0f);
  r_->rgba_sprite_renderer().DrawSprite(*gdi_logo_, gdi_pos_);
  r_->rgba_sprite_renderer().DrawSprite(*nod_logo_, nod_pos_);
  r_->rgba_sprite_renderer().DrawSprite(*eva_logo_, eva_pos_);

  WidgetUtils::DrawPanelPartial(bounds_, PanelSides::Edges,
                                border_top_.get(), border_bottom_.get(), border_left_.get(), border_right_.get(),
                                corner_top_left_.get(), corner_top_right_.get(), corner_bottom_left_.get(), corner_bottom_right_.get(),
                                nullptr);
  auto bar_y = bounds_.height - 78;

  if (!setup_ && !r_->fonts().empty()) {
    loading_font_ = r_->fonts().at("BigBold").get();
    loading_text_ = load_info_["Text"];
    loading_pos_ = { (bounds_.width - loading_font_->Measure(loading_text_).width) / 2, bar_y };

    version_font_ = r_->fonts().at("Regular").get();
    auto version_size = version_font_->Measure(version_text_);
    version_pos_ = { bounds_.width - 107 - version_size.width / 2, 115 - version_size.height / 2 };

    setup_ = true;
  }

  if (loading_font_ != nullptr) {
    loading_font_->DrawText(loading_text_, loading_pos_, Color::Gray);
  }
  if (version_font_ != nullptr) {
    version_font_->DrawTextWithContrast(version_text_, version_pos_, Color::White, Color::Black, 2);
  }

  for (auto i = 0; i <= 8; ++i) {
    const auto& block = load_tick_ == i ? bright_block_ : dim_block_;
    r_->rgba_sprite_renderer().DrawSprite(*block, { bounds_.width / 2 - 114 - i * 32, bar_y });
    r_->rgba_sprite_renderer().DrawSprite(*block, { bounds_.width / 2 + 114 + i * 32, bar_y });
  }

  r_->EndFrame(nih_);
}

}
}
}
