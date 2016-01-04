#pragma once

#include "cnc/stop_watch.h"

#define PERF_TIMER(name, body) \
{ \
  PerfTimer perf_timer(name); \
  body \
}

namespace cnc {

class PerfTimer {
public:
  PerfTimer(const std::string& name, float threshold_ms = 0);
  ~PerfTimer();

private:
  struct Report {
    void Write() const;
    std::string Indentation() const;

    std::string name;
    int64_t elapsed_ms = 0;
    int32_t depth = 0;
    std::vector<Report> children;
  };

  Report MakeReport(int64_t elapsed_ms) const;

  std::string name_;
  float threshold_ms_;
  int32_t depth_ = 0;
  PerfTimer* parent_;
  StopWatch stop_watch_;
  std::vector<Report> child_reports_;
};

}
