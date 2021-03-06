#pragma once

#include "cnc/cache.h"
#include "cnc/point.h"

namespace cnc {

struct Point;

class TapHistory {
public:
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
  
  explicit TapHistory(TimePoint now);
  int32_t GetTapCount(const Point& xy);
  int32_t LastTapCount() const;

private:
  static bool CloseEnough(const std::pair<TimePoint, Point>&a, const std::pair<TimePoint, Point>& b);

  std::pair<TimePoint, Point> first_release_;
  std::pair<TimePoint, Point> second_release_;
  std::pair<TimePoint, Point> third_release_;
};

class MultiTapDetection {
public:
  MultiTapDetection() = delete;

  static int32_t DetectFromMouse(char button, const Point& xy);
  static int32_t InfoFromMouse(char button);

private:
  static Cache<char, TapHistory> click_history_cache_;
};

}
