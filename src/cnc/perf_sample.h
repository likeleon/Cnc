#pragma once

#define PERF_SAMPLE(name, body) \
{ \
  PerfSample name(#name); \
  (name); \
  body \
}

namespace cnc {

class PerfSample {
public:
  PerfSample(const std::string& item);
  ~PerfSample();

private:
  std::string item_;
  std::clock_t ticks_;
};

}