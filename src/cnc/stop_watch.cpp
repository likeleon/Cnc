#include "cnc/stdafx.h"
#include "cnc/stop_watch.h"

namespace cnc {

StopWatch::StopWatch()
  : start_(std::chrono::high_resolution_clock::now()) {
}

int64_t StopWatch::ElapsedMilliseconds() const {
  auto elapsed = std::chrono::high_resolution_clock::now() - start_;
  return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
}

}