#include "cnc/stdafx.h"
#include "cnc/multi_tap_detection.h"

namespace cnc {

Cache<char, TapHistory> MultiTapDetection::click_history_cache_([](char /*c*/) {
  return TapHistory(std::chrono::steady_clock::now() - std::chrono::seconds(1));
});

TapHistory::TapHistory(TapHistory::TimePoint now)
  : first_release_(std::make_pair(now, Point::Zero))
  , second_release_(std::make_pair(now, Point::Zero))
  , third_release_(std::make_pair(now, Point::Zero)) {
}

int32_t TapHistory::GetTapCount(const Point& xy) {
  first_release_ = second_release_;
  second_release_ = third_release_;
  third_release_ = std::make_pair(std::chrono::steady_clock::now(), xy);

  if (!CloseEnough(third_release_, second_release_)) {
    return 1;
  }
  if (!CloseEnough(second_release_, first_release_)) {
    return 2;
  }

  return 3;
}

int32_t TapHistory::LastTapCount() const {
  if (!CloseEnough(third_release_, second_release_)) {
    return 1;
  }
  if (!CloseEnough(second_release_, first_release_)) {
    return 2;
  }

  return 3;
}

bool TapHistory::CloseEnough(const std::pair<TimePoint, Point>&a, const std::pair<TimePoint, Point>& b) {
  return a.first - b.first < std::chrono::milliseconds(250)
    && (a.second - b.second).Length() < 4;
}

int32_t MultiTapDetection::DetectFromMouse(char button, const Point& xy) {
  return click_history_cache_[button].GetTapCount(xy);
}

int32_t MultiTapDetection::InfoFromMouse(char button) {
  return click_history_cache_[button].LastTapCount();
}

}
