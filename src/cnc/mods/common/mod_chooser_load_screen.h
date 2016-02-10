#pragma once

#include "cnc/iload_screen.h"
#include "cnc/rectangle.h"
#include "cnc/sprite.h"

namespace cnc {
namespace mods {
namespace common {

class ModChooserLoadScreen : public ILoadScreen {
public:
  void Init(const Manifest& m, const std::map<std::string, std::string>& info) override;
  void Display() override;
  void StartGame(const Arguments& args) override;

private:
  std::unique_ptr<Sprite> sprite_;
  Rectangle bounds_;
};

}
}
}