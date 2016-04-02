#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/actor_lost_notification.h"
#include "cnc/field_load_info.h"
#include "cnc/error.h"

namespace cnc {
namespace mods {
namespace common {

std::vector<std::type_index> ActorLostNotificationInfo::BaseTypes() const {
  return{ typeid(ActorLostNotificationInfo) };
}

std::vector<FieldLoadInfo> ActorLostNotificationInfo::GetLoadInfo() const {
  return{
    { "Notification", TypeFieldInfo(&ActorLostNotificationInfo::notification_) },
    { "NotifyAll", TypeFieldInfo(&ActorLostNotificationInfo::notify_all_) },
  };
};

TypeExposablePtr ActorLostNotificationInfo::Create() {
  return std::make_shared<ActorLostNotification>(SharedFromBase<ActorLostNotificationInfo>());
}

ActorLostNotification::ActorLostNotification(std::shared_ptr<ActorLostNotificationInfo> info)
  : info_(info) {
}

std::vector<std::type_index> ActorLostNotification::Interfaces() const {
  return{ typeid(INotifyKilled) };
}

std::vector<std::type_index> ActorLostNotification::BaseTypes() const {
  return{ typeid(ITypeExposable) };
}

void ActorLostNotification::Killed(Actor& /*self*/, const AttackInfo& /*e*/) {
  throw Error(MSG("Not impleted"));
}

}
}
}
