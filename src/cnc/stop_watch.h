#pragma once

namespace cnc {

class CNC_API StopWatch {
public:
  StopWatch();

  void Restart();
  int64_t ElapsedMilliseconds() const;

private:
  std::chrono::time_point<std::chrono::steady_clock> start_;
};

}
