#pragma once

#include "cnc/perf_item.h"

namespace cnc {

class PerfHistory {
public:
  static void Increment(const std::string& name, double x);
  static void Tick();
  static PerfItem& Item(const std::string& name);

private:
  static std::unordered_map<std::string, PerfItem> items_;
};

}
