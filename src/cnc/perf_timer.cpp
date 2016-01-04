#include "cnc/stdafx.h"
#include "cnc/perf_timer.h"
#include "cnc/stop_watch.h"
#include "cnc/log.h"

namespace cnc {

static thread_local PerfTimer* parent_thread_local_;

static const int32_t Digits = 6;
static const std::string FormatSeparation = " ms ";
static const std::string IndentationString = "|   ";

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

static std::string FormatString(int64_t elapsed_ms, const std::string& text) {
  std::ostringstream oss;
  oss << std::setw(Digits) << std::to_string(elapsed_ms) << FormatSeparation << text;
  return oss.str();
}

static std::string GetHeader(const std::string& indentation, const std::string& label) {
  return std::string(Digits + FormatSeparation.length(), ' ') + indentation + label;
}

static std::string GetFooter(const std::string& indentation) {
  return indentation + std::string(std::max(15U, 50 - indentation.length()), '-');
}

void PerfTimer::Write() const {
  if (!children_.empty()) {
    Log::Write("perf", GetHeader(Indentation(), name_));
    for (const auto& child : children_) {
      child.Write();
    }
    Log::Write("perf", FormatString(elapsed_ms_, GetFooter(Indentation())));
  } else {
    Log::Write("perf", FormatString(elapsed_ms_, Indentation() + name_));
  }
}

std::string PerfTimer::Indentation() const {
  std::string res;
  for (auto i = 0; i < depth_; ++i) {
    res += IndentationString;
  }
  return res;
}

}
