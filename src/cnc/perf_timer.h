#pragma once

#include "cnc/stop_watch.h"

namespace cnc {

class PerfTimer {
public:
  PerfTimer(const std::string& name, float threshold_ms = 0);
  ~PerfTimer();
  
private:
  void Write();

  std::string name_;
  float threshold_ms_;
  int32_t depth_ = 0;
  PerfTimer* parent_;
  StopWatch stop_watch_;
  int64_t elapsed_ms_ = 0;
  std::vector<PerfTimer> children_;
};

}
