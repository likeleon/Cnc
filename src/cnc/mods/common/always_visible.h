#pragma once

#include "cnc/traits_interfaces.h"

namespace cnc {
namespace mods {
namespace common {

class AlwaysVisible;

class AlwaysVisibleInfo : public TraitInfo<AlwaysVisible>, public IDefaultVisibilityInfo {
public:
  std::deque<std::type_index> Types() const override;
  std::vector<FieldLoadInfo> GetLoadInfo() const override { return{}; }
};

class AlwaysVisible : public IDefaultVisibility {
public:
  std::deque<std::type_index> Types() const override;
  bool IsVisible(Actor& self, Player& by_player) override;
};

}
}
}
