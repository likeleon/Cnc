#pragma once

namespace cnc {

class PerfItem {
public:
  PerfItem(const std::string& name);

  void IncreaseVal(double amount);
  void Tick();
  std::vector<double> Samples();
  double Average(size_t count);
  double LastValue();

  bool has_normal_tick() const { return has_normal_tick_; }
  void set_has_normal_tick(bool value) { has_normal_tick_ = value; }

private:
  std::string name_;
  std::array<double, 100> samples_ = {};
  double val_ = 0.0;
  size_t head_ = 1;
  size_t tail_ = 0;
  bool has_normal_tick_ = true;
};

}
