#include "cnc/stdafx.h"
#include "cnc/perf_history.h"

namespace cnc {

std::unordered_map<std::string, PerfItem> PerfHistory::items_;

void PerfHistory::Increment(const std::string& name, double x) {
  auto iter = items_.find(name);
  if (iter == items_.end()) {
    iter = items_.emplace(name, PerfItem(name)).first;
  }

  PerfItem& perf_item = iter->second;
  perf_item.IncreaseVal(x);
}

void PerfHistory::Tick() {
  for (auto& kvp : items_) {
    PerfItem& item = kvp.second;
    if (item.has_normal_tick()) {
      item.Tick();
    }
  }
}

PerfItem& PerfHistory::Item(const std::string& name) {
  return items_.at(name);
}

}
