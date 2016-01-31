#include "cnc/stdafx.h"
#include "cnc/multi_tap_detection.h"

namespace cnc {

TapHistory::TapHistory(TapHistory::TimePoint now) {
  first_release_ = second_release_ = third_release_ = std::make_pair(now, Point::Zero);
}

bool TapHistory::CloseEnough(const std::pair<TimePoint, Point>&a, const std::pair<TimePoint, Point>& b) {
  return a.first - b.first < std::chrono::milliseconds(250)
    && (a.second - b.second).
}

int32_t MultiTapDetection::DetectFromMouse(char button, const Point& xy) {
  return int32_t();
}

}
