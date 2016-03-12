#pragma once

#include "cnc/widget.h"
#include "cnc/size.h"

namespace cnc {
namespace mods {
namespace common {

class ProgressBarWidget : public Widget {
public:
  ProgressBarWidget();
  ProgressBarWidget(const ProgressBarWidget& other);

  WidgetPtr Clone() const override;
  void Draw() override;

  // TODO void Tick() override;

  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  std::string background_ = "progressbar-bg";
  std::string bar_ = "progressbar-thumb";
  Size bar_margin_ = Size(2, 2);

  int32_t percentage_ = 0;
  bool indeterminate_ = false;

  Func<int32_t> get_percentage_;
  Func<bool> is_indeterminate_;

private:
  float offset_ = 0.0f;
  float tick_step = 0.04f;
  bool was_indeterminate_ = false;
};

}
}
}
