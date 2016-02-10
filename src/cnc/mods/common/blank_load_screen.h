#pragma once

#include "cnc/iload_screen.h"

namespace cnc {
namespace mods {
namespace common {

class BlankLoadScreen : public ILoadScreen {
public:
  void Init(const Manifest& m, const std::map<std::string, std::string>& info) override;
  void Display() override;
  void StartGame(const Arguments& args)  override;
};

}
}
}
