#pragma once

namespace cnc {

class CNC_API ChromeLogic {
public:
  virtual void Tick() {}
};

using ChromeLogicUniquePtr = std::unique_ptr<ChromeLogic>;

}
