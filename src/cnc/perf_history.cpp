#include "cnc/stdafx.h"
#include "cnc/perf_history.h"

namespace cnc {

Cache<std::string, PerfItem> PerfHistory::items_([](const std::string& s) { return PerfItem(s); });

void PerfHistory::Increment(const std::string& name, double x) {
  Items(name).IncreaseVal(x);
}

void PerfHistory::Tick() {
  for (auto& kvp : items_) {
    PerfItem& item = kvp.second;
    if (item.has_normal_tick()) {
      item.Tick();
    }
  }
}

PerfItem& PerfHistory::Items(const std::string& name) {
  return items_[name];
}

}
