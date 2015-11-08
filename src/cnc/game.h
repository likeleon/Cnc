#pragma once

namespace cnc {

class Arguments;

enum class RunStatus {
  Error = -1,
  Success = 0,
  Running = std::numeric_limits<int>::max()
};

class Game {
public:
  static void Initialize(const Arguments& args);
  static RunStatus Run();
};

}