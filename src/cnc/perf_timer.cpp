#include "cnc/stdafx.h"
#include "cnc/perf_timer.h"
#include "cnc/stop_watch.h"
#include "cnc/log.h"

namespace cnc {

static thread_local PerfTimer* parent_thread_local_;

PerfTimer::PerfTimer(const std::string & name, float threshold_ms)
  : name_(name), threshold_ms_(threshold_ms) {
  parent_ = parent_thread_local_;
  depth_ = (parent_ == nullptr) ? 0 : parent_->depth_ + 1;
  parent_thread_local_ = this;
}

PerfTimer::~PerfTimer() {
  elapsed_ms_ = stop_watch_.ElapsedMilliseconds();

  parent_thread_local_ = parent_;

  if (parent_ == nullptr) {
    Write();
  } else if (elapsed_ms_ > threshold_ms_) {
    parent_->children_.emplace_back(*this);
  }
}

void PerfTimer::Write() {
  if (!children_.empty()) {
  } else {
    Log::Write("perf", )
  }
}

}
