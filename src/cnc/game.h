#pragma once

namespace cnc {

class Arguments;

enum class RunStatus : int8_t {
  Error = -1,
  Success = 0,
  Running = std::numeric_limits<int8_t>::max()
};

class Game {
public:
  static void Initialize(const Arguments& args);
  static RunStatus Run();
};

}