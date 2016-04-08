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

class AlwaysVisible : public ITypeExposable, public IDefaultVisibility {
public:
  bool IsVisible(Actor& self, Player& by_player) override;
  
  std::deque<std::type_index> Types() const override;
};

}
}
}
