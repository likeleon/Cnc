#pragma once

#include "cnc/perf_item.h"
#include "cnc/cache.h"

namespace cnc {

class PerfHistory {
public:
  static void Increment(const std::string& name, double x);
  static void Tick();
  static PerfItem& Items(const std::string& name);

private:
  static Cache<std::string, PerfItem> items_;
};

}
