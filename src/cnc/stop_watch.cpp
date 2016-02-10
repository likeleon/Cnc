#include "cnc/stdafx.h"
#include "cnc/stop_watch.h"

namespace cnc {

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;

StopWatch::StopWatch()
  : start_(high_resolution_clock::now()) {
}

void StopWatch::Restart() {
  start_ = high_resolution_clock::now();
}

int64_t StopWatch::ElapsedMilliseconds() const {
  auto elapsed = high_resolution_clock::now() - start_;
  return duration_cast<milliseconds>(elapsed).count();
}

}