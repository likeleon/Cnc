#pragma once

namespace cnc {

class CNC_API StopWatch {
public:
  StopWatch();

  int64_t ElapsedMilliseconds() const;

private:
  std::chrono::time_point<std::chrono::steady_clock> start_;
};

}
