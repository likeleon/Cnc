#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/always_visible.h"

namespace cnc {
namespace mods {
namespace common {

std::deque<std::type_index> AlwaysVisibleInfo::Types() const {
  auto types = TraitInfo<AlwaysVisible>::Types();
  types.emplace_front(typeid(AlwaysVisibleInfo));
  types.emplace_back(typeid(IDefaultVisibilityInfo));
  return types;
}

std::deque<std::type_index> AlwaysVisible::Types() const {
  auto types = IDefaultVisibility::Types();
  types.emplace_front(typeid(AlwaysVisible));
  return types;
}

bool AlwaysVisible::IsVisible(Actor& /*self*/, Player& /*by_player*/) {
  return true;
}

}
}
}
