#include "cnc/stdafx.h"
#include "cnc/action_queue.h"

namespace cnc {

ActionQueue::DelayedAction::DelayedAction(int64_t time, ActionQueue::Action action)
  : time(time), action(action) {
}

void ActionQueue::Add(const Action& a, int64_t desired_time) {
  // TODO: lock?
  actions_.emplace(desired_time, a);
}

void ActionQueue::PerformActions(int64_t current_time) {
  // TODO: lock?
  auto itup = actions_.upper_bound({ current_time, nullptr });
  if (itup == actions_.begin()) {
    return;
  }

  std::vector<DelayedAction> pending_actions(actions_.begin(), itup);
  actions_.erase(actions_.begin(), itup);

  for (const auto& a : pending_actions) {
    a.action();
  }
}

}
