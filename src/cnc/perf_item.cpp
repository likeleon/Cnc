#include "cnc/stdafx.h"
#include "cnc/perf_item.h"

namespace cnc {

PerfItem::PerfItem(const std::string& name)
  : name_(name) {
}

void PerfItem::IncreaseVal(double amount) {
  val_ += amount;
}

void PerfItem::Tick() {
  samples_[head_++] = val_;
  if (head_ == samples_.size()) {
    head_ = 0;
  }
  if (head_ == tail_ && ++tail_ == samples_.size()) {
    tail_ = 0;
  }
  val_ = 0.0;
}

std::vector<double> PerfItem::Samples() {
  std::vector<double> ret;
  size_t n = head_;
  while (n != tail_) {
    --n;
    if (n < 0) {
      n = samples_.size() - 1;
    }
    ret.emplace_back(samples_[n]);
  }
  return ret;
}

double PerfItem::Average(size_t count) {
  size_t i = 0;
  size_t n = head_;
  double sum = 0;
  while (i < count && n != tail_) {
    if (--n < 0) {
      n = samples_.size() - 1;
    }
    sum += samples_[n];
    ++i;
  }
  return i == 0 ? sum : sum / i;
}

double PerfItem::LastValue() {
  size_t n = head_;
  if (--n < 0) {
    n = samples_.size() - 1;
  }
  return samples_[n];
}

}
