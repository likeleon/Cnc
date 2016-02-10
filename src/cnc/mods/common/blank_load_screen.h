#pragma once

#include "cnc/iload_screen.h"
#include "cnc/input_handler.h"

namespace cnc {
namespace mods {
namespace common {

class CNC_MODS_COMMON_API BlankLoadScreen : public ILoadScreen {
public:
  void Init(const Manifest& m, const std::map<std::string, std::string>& info) override;
  void Display() override;
  void StartGame(const Arguments& args) override;

private:
  NullInputHandler nih_;
};

}
}
}
