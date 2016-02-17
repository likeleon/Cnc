#pragma once

namespace cnc {

class CNC_API ChromeLogic {
public:
  virtual void Tick() {}
};

using ChromeLogicPtr = std::shared_ptr<ChromeLogic>;

}
