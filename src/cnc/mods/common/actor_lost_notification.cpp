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

std::shared_ptr<void> ActorLostNotificationInfo::Create() {
  return std::make_shared<ActorLostNotification>(SharedFromBase<ActorLostNotificationInfo>());
}

ActorLostNotification::ActorLostNotification(std::shared_ptr<ActorLostNotificationInfo> info)
  : info_(info) {
}

void ActorLostNotification::Killed(Actor& /*self*/, const AttackInfo& /*e*/) {
  throw Error(MSG("Not impleted"));
}

}
}
}
