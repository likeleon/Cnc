#include "cnc/stdafx.h"
#include "cnc/perf_sample.h"
#include "cnc/perf_history.h"

namespace cnc {

PerfSample::PerfSample(const std::string& item)
: item_(item), ticks_(std::clock()) {
}

PerfSample::~PerfSample() {
  double elapsed = 1000.0 * std::max(0L, std::clock() - ticks_) / CLOCKS_PER_SEC;
  PerfHistory::Increment(item_, elapsed);
}

}
