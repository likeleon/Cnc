#pragma once

#include "cnc/traits_interfaces.h"

namespace cnc {
namespace mods {
namespace common {

class ActorLostNotificationInfo : public ITraitInfo {
public:
  std::vector<std::type_index> BaseTypes() const override;
  std::vector<FieldLoadInfo> GetLoadInfo() const override;
  TypeExposablePtr Create() override;

  std::string notification_ = "UnitLost";
  bool notify_all_ = false;
};

class ActorLostNotification : public ITypeExposable, public INotifyKilled {
public:
  explicit ActorLostNotification(std::shared_ptr<ActorLostNotificationInfo> info);
  
  void Killed(Actor& self, const AttackInfo& e) override;

  std::vector<std::type_index> Interfaces() const override;
  std::vector<std::type_index> BaseTypes() const override;

  std::shared_ptr<ActorLostNotificationInfo> info_;
};

}
}
}
