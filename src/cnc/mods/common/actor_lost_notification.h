#pragma once

#include "cnc/traits_interfaces.h"

namespace cnc {
namespace mods {
namespace common {

class ActorLostNotificationInfo : public ITraitInfo {
public:
  std::deque<std::type_index> Types() const override;
  std::vector<FieldLoadInfo> GetLoadInfo() const override;
  ITraitPtr Create(const ActorInitializer& init) override;

  std::string notification_ = "UnitLost";
  bool notify_all_ = false;
};

class ActorLostNotification : public INotifyKilled {
public:
  explicit ActorLostNotification(std::shared_ptr<ActorLostNotificationInfo> info);
  
  void Killed(Actor& self, const AttackInfo& e) override;

  std::deque<std::type_index> Types() const override;

  std::shared_ptr<ActorLostNotificationInfo> info_;
};

}
}
}
