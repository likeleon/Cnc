#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/actor_lost_notification.h"
#include "cnc/field_load_info.h"
#include "cnc/error.h"

namespace cnc {
namespace mods {
namespace common {

std::deque<std::type_index> ActorLostNotificationInfo::Types() const {
  return{ typeid(ActorLostNotificationInfo) };
}

std::vector<FieldLoadInfo> ActorLostNotificationInfo::GetLoadInfo() const {
  return{
    { "Notification", TypeFieldInfo(&ActorLostNotificationInfo::notification_) },
    { "NotifyAll", TypeFieldInfo(&ActorLostNotificationInfo::notify_all_) },
  };
};

ITraitPtr ActorLostNotificationInfo::Create(const ActorInitializer& /*init*/) {
  return std::make_shared<ActorLostNotification>(SharedFromBase<ActorLostNotificationInfo>());
}

ActorLostNotification::ActorLostNotification(std::shared_ptr<ActorLostNotificationInfo> info)
  : info_(info) {
}

std::deque<std::type_index> ActorLostNotification::Types() const {
  return{ typeid(ActorLostNotification), typeid(INotifyKilled) };
}

void ActorLostNotification::Killed(Actor& /*self*/, const AttackInfo& /*e*/) {
  throw Error(MSG("Not impleted"));
}

}
}
}
