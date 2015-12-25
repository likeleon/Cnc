#include "cnc/stdafx.h"
#include "cnc/perf_history.h"

namespace cnc {

std::unordered_map<std::string, PerfItem> PerfHistory::items_;

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
  auto iter = items_.find(name);
  if (iter == items_.end()) {
    iter = items_.emplace(name, PerfItem(name)).first;
  }
  return iter->second;
}

}
