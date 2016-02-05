#pragma once

namespace cnc {

class ActionQueue {
public:
  using Action = std::function<void()>;

  void Add(const Action& a, int64_t desired_time);
  void PerformActions(int64_t current_time);

private:
  struct DelayedAction {
    DelayedAction(int64_t time, Action action);

    int64_t time;
    Action action;
  };

  struct DelayedActionComparer {
    bool operator()(const DelayedAction& a, const DelayedAction& b) const {
      return a.time < b.time;
    }
  };

  std::set<DelayedAction, DelayedActionComparer> actions_;
};

}
